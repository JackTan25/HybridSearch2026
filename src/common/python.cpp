

module;

module python_instance;
#if 0
#include "Python.h"
namespace hybridsearch {

static PyThreadState *saved_state = nullptr;

void PythonInstance::Init() {
    if (!Py_IsInitialized()) {
        Py_InitializeEx(0);
    }

    PyEval_InitThreads();
    saved_state = PyEval_SaveThread();
}

void PythonInstance::UnInit() {
    PyEval_RestoreThread(saved_state);
    saved_state = nullptr;

    if (Py_IsInitialized()) {
        Py_FinalizeEx();
    }

}

}
#endif