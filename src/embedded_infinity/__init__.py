try:
    from hybridsearch_embedded.embedded_hybridsearch_ext import *
except ImportError as e:
    raise ImportError(f"Could not import the embedded_hybridsearch_ext module: {e}")

