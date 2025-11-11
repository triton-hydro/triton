
#pragma once

#ifdef ENSEMBLE_BUILD

#include "yaml-cpp/yaml.h"
#include <fstream>

// Specify the Ensemble struct first
namespace ensify {
  struct Ensemble {
    int                       rank_beg;
    int                       rank_end;
    std::vector<std::string>  args;
    std::string               out_file;
    std::string               err_file;
  };
}

// provide a template specialization to read in an ensemble from YAML
namespace YAML {
  template<> struct convert<ensify::Ensemble> {
    static bool decode(const Node& node, ensify::Ensemble& rhs) {
      if (!node.IsSequence() || node.size() != 5) { return false; }
      rhs.rank_beg = node[0].as<int>();
      rhs.rank_end = node[1].as<int>();
      rhs.args     = node[2].as<std::vector<std::string>>();
      rhs.out_file = node[3].as<std::string>();
      rhs.err_file = node[4].as<std::string>();
      return true;
    }
  };
}

namespace ensify {
  struct Ensify {
    std::string               exe_name;
    MPI_Comm                  ensemble_comm;
    std::vector<std::string>  ensemble_args;
    std::ofstream             ostr;
    std::ofstream             estr;

    void init(int argc , char **argv , MPI_Comm parent_comm ) {
      if (argc <= 1) { throw std::runtime_error("ERROR: Must pass the input YAML filename as a parameter"); }
      exe_name = std::string(argv[0]);
      init( std::string(argv[1]) , parent_comm );
    }

    void init( std::string fname , MPI_Comm parent_comm ) {
      if (exe_name.empty()) exe_name = std::string("a.out");
      int parent_rank;
      mpicheck( MPI_Comm_rank( parent_comm , &parent_rank ) );
      std::string inFile(fname);
      YAML::Node config = YAML::LoadFile(inFile);
      if ( !config ) { throw std::runtime_error("ERROR: Invalid YAML input file"); }
      auto ensembles = config["ensembles"].as<std::vector<Ensemble>>();
      for (int i = 0; i < ensembles.size(); i++) {
        bool active = parent_rank >= ensembles[i].rank_beg && parent_rank <= ensembles[i].rank_end;
        MPI_Comm newcomm;
        mpicheck( MPI_Comm_split( parent_comm , active ? 1 : 0 , parent_rank , &newcomm ) );
        if (active) {
          ensemble_comm = newcomm;
          ensemble_args = ensembles[i].args;
          ostr = std::ofstream(ensembles[i].out_file);
          estr = std::ofstream(ensembles[i].err_file);
          std::cout.rdbuf(ostr.rdbuf());
          std::cerr.rdbuf(estr.rdbuf());
        } else {
          mpicheck( MPI_Comm_free( &newcomm ) );
        }
      }
    }

    void finalize() {
      ostr.close();
      estr.close();
      mpicheck( MPI_Comm_free( &ensemble_comm ) );
    }

    MPI_Comm                 get_ensemble_comm  () const { return ensemble_comm; }
    std::vector<std::string> get_ensemble_argvec() const { return ensemble_args; }
    int                      get_ensemble_argc  () const { return ensemble_args.size()+1; }

    char ** get_ensemble_argv() const {
      auto ret = (char **) malloc( (ensemble_args.size()+1)*sizeof(char *) );
      ret[0] = const_cast<char *>(exe_name.data());
      for (int i=1; i <= ensemble_args.size(); i++) { ret[i] = const_cast<char *>(ensemble_args[i-1].data()); }
      return ret;
    }

    void mpicheck( int e ) const {
      if (e == MPI_SUCCESS ) return;
      char estring[MPI_MAX_ERROR_STRING];
      int len;
      MPI_Error_string(e, estring, &len);
      printf("MPI Error: %s\n", estring);
      std::cout << std::endl;
      std::cerr << std::endl;
      throw std::runtime_error("MPI Error");
    }
  };

  // Create a static lifetime namespace scoped object
  Ensify ensify;

  // Create short convenience functions for the user
  void init(int argc, char **argv, MPI_Comm comm = MPI_COMM_WORLD ) { ensify.init(argc,argv,comm); }
  void init(std::string fname , MPI_Comm comm = MPI_COMM_WORLD ) { ensify.init(fname,comm); }
  void finalize() { ensify.finalize(); }
  MPI_Comm                 comm  () { return ensify.get_ensemble_comm  (); }
  std::vector<std::string> argvec() { return ensify.get_ensemble_argvec(); }
  int                      argc  () { return ensify.get_ensemble_argc  (); }
  char **                  argv  () { return ensify.get_ensemble_argv  (); }
}

#define ENSIFY_COMM_WORLD ensify::comm()

#else

#define ENSIFY_COMM_WORLD MPI_COMM_WORLD

#endif
