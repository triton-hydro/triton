import os
import sys
import datetime

# --- Path setup ---
# Add the project root to sys.path so Sphinx can find any Python modules if needed
sys.path.insert(0, os.path.abspath('..')) # Go up one level from 'doc' to 'triton'

# --- Project information ---
project = 'TRITON Documentation'
copyright = f'{datetime.datetime.now().year}, T.B.D.'
author = 'T.B.D.'
release = '2.1.1'
version = '2.1'

# --- General configuration ---
extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.doctest',
    'sphinx.ext.intersphinx',
    'sphinx.ext.todo',
    'sphinx.ext.coverage',
    'sphinx.ext.mathjax',
    'sphinx.ext.ifconfig',
    'sphinx.ext.viewcode',
    'sphinx.ext.githubpages',
#    'breathe', # Add breathe here
#    'exhale',  # Add exhale here
    'sphinx_design',
]

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

# --- Options for HTML output ---
html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']

## --- Breathe Configuration ---
## Point Breathe to the Doxygen XML output
## This path is relative to the conf.py file (i.e., doc/doxygen_output/xml)
#breathe_projects = {
#    "TRITON": os.path.abspath(os.path.join(os.path.dirname(__file__), 'doxygen_output/xml'))
#}
#breathe_default_project = "TRITON"
#breathe_domain_by_extension = {
#    "h": "cpp",
#    "c": "c",
#    "cpp": "cpp",
#    "hpp": "cpp",
#}
#
## --- Exhale Configuration ---
#exhale_args = {
#    "containmentFolder": "./api",
#    "rootFileName": "library_root.rst",
#    "doxygenStripFromPath": "../src", # Path to strip from Doxygen source files for cleaner paths (relative to doc/)
#    "createTreeView": True,
#    "exhaleExecutesDoxygen": False, # Important: Makefile runs Doxygen, not Exhale
#    "verboseBuild": False,
#    "unabridgedOrphanKinds": [],
##    "unifiedSecondaryDirectives": True,
##    "fullApiSubTree": True,
#}

# Define the master document
master_doc = 'index'

# Auto-generate 'api.rst' if it doesn't exist, and include the Exhale root.
if not os.path.exists(os.path.join(os.path.dirname(__file__), 'api.rst')):
    with open(os.path.join(os.path.dirname(__file__), 'api.rst'), 'w') as f:
        f.write(".. _api:\n\n")
        f.write("API Reference\n")
        f.write("=============\n\n")
        f.write(".. toctree::\n")
        f.write("   :maxdepth: 2\n\n")
        f.write(f"   api/{exhale_args['rootFileName']}\n")
