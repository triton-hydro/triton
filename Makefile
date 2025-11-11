
# --- Documentation Variables ---
DOCS_DIR = doc
DOXYFILE = $(DOCS_DIR)/Doxyfile
DOXYGEN_XML_DIR = $(DOCS_DIR)/doxygen_output/xml
SPHINX_BUILD_DIR = $(DOCS_DIR)/_build/html

#$(info DEBUG: DOCS_DIR is: "$(DOCS_DIR)")
#$(info DEBUG: DOXYGEN_XML_DIR is: "$(DOXYGEN_XML_DIR)")
#$(info DEBUG: Expected Target is: "$(DOXYGEN_XML_DIR)/index.xml")

# Python executable for Sphinx (assuming virtual environment in .venv_triton)
# Adjust if your Python setup is different
PYTHON = $(shell command -v python3 || command -v python)  
VENV_PYTHON = $(CURDIR)/.venv_triton/bin/python

# Ensure the virtual environment's python is used if it exists
ifeq ($(wildcard $(VENV_PYTHON)),)
    # Virtual environment not found, fall back to system python
    PYTHON_EXECUTABLE = $(PYTHON)
else
    PYTHON_EXECUTABLE = $(VENV_PYTHON)
endif

# --- Documentation Targets ---

.PHONY: doc doxygen sphinx clean_doc

# Main documentation target: builds Doxygen, then Sphinx
doc: sphinx

# Doxygen target
# The BYPRODUCTS here tells Make that if this file doesn't exist/is older, re-run doxygen.
# This makes it smarter than just running "doxygen" every time.
# We also change directory to doc/ because Doxyfile paths are relative to doc/.
$(DOXYGEN_XML_DIR)/index.xml: $(wildcard src/**/*.h src/**/*.cpp) $(DOXYFILE)
	@echo "Generating Doxygen XML documentation..."
	$(info DEBUG: Inside rule. Target is: "$@")
	$(info DEBUG: Inside rule. Target Directory ($(D)) is: "$(@D)")
	mkdir -p $(@D) # Ensure the XML output directory exists
	cd $(DOCS_DIR) && doxygen $(notdir $(DOXYFILE)) # Execute doxygen from doc/

# Sphinx target
# Depends on the Doxygen XML output to ensure it's up-to-date
$(SPHINX_BUILD_DIR)/index.html: $(wildcard $(DOCS_DIR)/*.rst $(DOCS_DIR)/**/*.rst $(DOCS_DIR)/*.py) $(DOXYGEN_XML_DIR)/index.xml
	@echo "Generating Sphinx HTML documentation..."
	mkdir -p $(@D) # Ensure the Sphinx build directory exists
	$(PYTHON_EXECUTABLE) -m sphinx -b html $(DOCS_DIR) $(SPHINX_BUILD_DIR)

sphinx: $(SPHINX_BUILD_DIR)/index.html

doxygen: $(DOXYGEN_XML_DIR)/index.xml

docker_build:
	docker build -t triton:v1 .

docker_run:
	mkdir -p output && \
	docker run --rm \
		-v "${PWD}/output:/app/triton/build/output" \
		triton:v1 triton_run.sh ./input/paraboloid/paraboloid.cfg

setup_pyenv:
	@echo ""
	@echo "Run the following commands to set up a virtual environment and install the Sphinx packages:"
	@echo ""
	@echo "python -m venv .venv_triton"
	@echo "source .venv_triton/bin/activate"
	@echo "pip install sphinx breathe exhale sphinx_rtd_theme sphinx_design"
	@echo ""
	@echo "You also need to install Doxygen, for example: sudo apt install doxygen"

# Clean documentation build artifacts
clean_doc:
	@echo "Cleaning documentation build files..."
	rm -rf $(DOCS_DIR)/doxygen_output
	rm -rf $(DOCS_DIR)/_build
