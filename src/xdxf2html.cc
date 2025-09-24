#include <Python.h>
#include "xdxf2html.h"

static PyObject *convert(PyObject *self, PyObject *args)
{
	const char *xdxf;
	const char *base_url_static_files;
	const char *base_url_lookup;

	if (!PyArg_ParseTuple(args, "sss", &xdxf, &base_url_static_files, &base_url_lookup))
	{
		return NULL;
	}

	std::string result;

	Py_BEGIN_ALLOW_THREADS
		dom d(xdxf);
		builder b(base_url_static_files, base_url_lookup);
		result = b.get_html(d.root);
	Py_END_ALLOW_THREADS

		PyObject *html = PyUnicode_DecodeUTF8(result.c_str(), result.length(), "strict");

	return html;
}

static PyMethodDef Xdxf2htmlMethods[] = {
	{"convert", convert, METH_VARARGS, "Convert XDXF to HTML"},
	{NULL, NULL, 0, NULL}};

static struct PyModuleDef xdxf2htmlmodule = {
	PyModuleDef_HEAD_INIT,
	"xdxf2html",
	NULL,
	-1,
	Xdxf2htmlMethods};

PyMODINIT_FUNC PyInit_xdxf2html(void)
{
	return PyModule_Create(&xdxf2htmlmodule);
}
