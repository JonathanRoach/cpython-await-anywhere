/* ------------------------------------------------------------------------

   _codecs -- Provides access to the codec registry and the builtin
              codecs.

   This module should never be imported directly. The standard library
   module "codecs" wraps this builtin module for use within Python.

   The codec registry is accessible via:

     register(search_function) -> None

     lookup(encoding) -> CodecInfo object

   The builtin Unicode codecs use the following interface:

     <encoding>_encode(Unicode_object[,errors='strict']) ->
        (string object, bytes consumed)

     <encoding>_decode(char_buffer_obj[,errors='strict']) ->
        (Unicode object, bytes consumed)

   These <encoding>s are available: utf_8, unicode_escape,
   raw_unicode_escape, latin_1, ascii (7-bit), mbcs (on win32).


Written by Marc-Andre Lemburg (mal@lemburg.com).

Copyright (c) Corporation for National Research Initiatives.

   ------------------------------------------------------------------------ */

#include "Python.h"
#include "pycore_codecs.h"        // _PyCodec_Lookup()
#include "pycore_unicodeobject.h" // _PyUnicode_EncodeCharmap

#ifdef MS_WINDOWS
#include <windows.h>
#endif

/*[clinic input]
module _codecs
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=e1390e3da3cb9deb]*/

#include "pycore_runtime.h"
#include "clinic/_codecsmodule.c.h"

/* --- Registry ----------------------------------------------------------- */

/*[clinic input]
@c_stack_frugal
_codecs.register
    search_function: object
    /

Register a codec search function.

Search functions are expected to take one argument, the encoding name in
all lower case letters, and either return None, or a tuple of functions
(encoder, decoder, stream_reader, stream_writer) (or a CodecInfo object).
[clinic start generated code]*/

static PyObject *
_codecs_register(PyObject *module, PyObject *search_function)
/*[clinic end generated code: output=d1bf21e99db7d6d3 input=e19e5e364c9be2f6]*/
{
    if (PyCodec_Register(search_function))
        return NULL;

    Py_RETURN_NONE;
}

/*[clinic input]
@c_stack_frugal
_codecs.unregister
    search_function: object
    /

Unregister a codec search function and clear the registry's cache.

If the search function is not registered, do nothing.
[clinic start generated code]*/

static PyObject *
_codecs_unregister(PyObject *module, PyObject *search_function)
/*[clinic end generated code: output=1f0edee9cf246399 input=3d7f95cecbffac88]*/
{
    if (PyCodec_Unregister(search_function) < 0) {
        return NULL;
    }

    Py_RETURN_NONE;
}

/*[clinic input]
@c_stack_frugal
_codecs.lookup
    encoding: str
    /

Looks up a codec tuple in the Python codec registry and returns a CodecInfo object.
[clinic start generated code]*/

static PyObject *
_codecs_lookup_impl(PyObject *module, const char *encoding)
/*[clinic end generated code: output=9f0afa572080c36d input=a519d8a6b97ab913]*/
{
    return _PyCodec_Lookup(encoding);
}

/*[clinic input]
@c_stack_frugal
_codecs.encode
    obj: object
    encoding: str(c_default="NULL") = "utf-8"
    errors: str(c_default="NULL") = "strict"

Encodes obj using the codec registered for encoding.

The default encoding is 'utf-8'.  errors may be given to set a
different error handling scheme.  Default is 'strict' meaning that encoding
errors raise a ValueError.  Other possible values are 'ignore', 'replace'
and 'backslashreplace' as well as any other name registered with
codecs.register_error that can handle ValueErrors.
[clinic start generated code]*/

static PyObject *
_codecs_encode_impl(PyObject *module, PyObject *obj, const char *encoding,
                    const char *errors)
/*[clinic end generated code: output=385148eb9a067c86 input=50fba40676d642d4]*/
{
    if (encoding == NULL)
        encoding = PyUnicode_GetDefaultEncoding();

    /* Encode via the codec registry */
    return PyCodec_Encode(obj, encoding, errors);
}

/*[clinic input]
@c_stack_frugal
_codecs.decode
    obj: object
    encoding: str(c_default="NULL") = "utf-8"
    errors: str(c_default="NULL") = "strict"

Decodes obj using the codec registered for encoding.

Default encoding is 'utf-8'.  errors may be given to set a
different error handling scheme.  Default is 'strict' meaning that encoding
errors raise a ValueError.  Other possible values are 'ignore', 'replace'
and 'backslashreplace' as well as any other name registered with
codecs.register_error that can handle ValueErrors.
[clinic start generated code]*/

static PyObject *
_codecs_decode_impl(PyObject *module, PyObject *obj, const char *encoding,
                    const char *errors)
/*[clinic end generated code: output=679882417dc3a0bd input=c99a6b86b1f0c402]*/
{
    if (encoding == NULL)
        encoding = PyUnicode_GetDefaultEncoding();

    /* Decode via the codec registry */
    return PyCodec_Decode(obj, encoding, errors);
}

/* --- Helpers ------------------------------------------------------------ */

static
PyObject *codec_tuple(PyObject *decoded,
                      Py_ssize_t len)
{
    if (decoded == NULL)
        return NULL;
    return Py_BuildValue("Nn", decoded, len);
}

/* --- String codecs ------------------------------------------------------ */
/*[clinic input]
@c_stack_frugal
_codecs.escape_decode
    data: Py_buffer(accept={str, buffer})
    errors: str(accept={str, NoneType}) = None
    /
[clinic start generated code]*/

static PyObject *
_codecs_escape_decode_impl(PyObject *module, Py_buffer *data,
                           const char *errors)
/*[clinic end generated code: output=505200ba8056979a input=dafadb69a1315942]*/
{
    PyObject *decoded = PyBytes_DecodeEscape(data->buf, data->len,
                                             errors, 0, NULL);
    return codec_tuple(decoded, data->len);
}

/*[clinic input]
@c_stack_frugal
_codecs.escape_encode
    data: object(subclass_of='&PyBytes_Type')
    errors: str(accept={str, NoneType}) = None
    /
[clinic start generated code]*/

static PyObject *
_codecs_escape_encode_impl(PyObject *module, PyObject *data,
                           const char *errors)
/*[clinic end generated code: output=4af1d477834bab34 input=37035b2d9deb22a4]*/
{
    Py_ssize_t size;
    Py_ssize_t newsize;
    PyObject *v;

    size = PyBytes_GET_SIZE(data);
    if (size > PY_SSIZE_T_MAX / 4) {
        PyErr_SetString(PyExc_OverflowError,
            "string is too large to encode");
            return NULL;
    }
    newsize = 4*size;
    v = PyBytes_FromStringAndSize(NULL, newsize);

    if (v == NULL) {
        return NULL;
    }
    else {
        Py_ssize_t i;
        char c;
        char *p = PyBytes_AS_STRING(v);

        for (i = 0; i < size; i++) {
            /* There's at least enough room for a hex escape */
            assert(newsize - (p - PyBytes_AS_STRING(v)) >= 4);
            c = PyBytes_AS_STRING(data)[i];
            if (c == '\'' || c == '\\')
                *p++ = '\\', *p++ = c;
            else if (c == '\t')
                *p++ = '\\', *p++ = 't';
            else if (c == '\n')
                *p++ = '\\', *p++ = 'n';
            else if (c == '\r')
                *p++ = '\\', *p++ = 'r';
            else if (c < ' ' || c >= 0x7f) {
                *p++ = '\\';
                *p++ = 'x';
                *p++ = Py_hexdigits[(c & 0xf0) >> 4];
                *p++ = Py_hexdigits[c & 0xf];
            }
            else
                *p++ = c;
        }
        *p = '\0';
        if (_PyBytes_Resize(&v, (p - PyBytes_AS_STRING(v)))) {
            return NULL;
        }
    }

    return codec_tuple(v, size);
}

/* --- Decoder ------------------------------------------------------------ */
/*[clinic input]
@c_stack_frugal
_codecs.utf_7_decode
    data: Py_buffer
    errors: str(accept={str, NoneType}) = None
    final: bool = False
    /
[clinic start generated code]*/

static PyObject *
_codecs_utf_7_decode_impl(PyObject *module, Py_buffer *data,
                          const char *errors, int final)
/*[clinic end generated code: output=0cd3a944a32a4089 input=9d855b852d2158c6]*/
{
    Py_ssize_t consumed = data->len;
    PyObject *decoded = PyUnicode_DecodeUTF7Stateful(data->buf, data->len,
                                                     errors,
                                                     final ? NULL : &consumed);
    return codec_tuple(decoded, consumed);
}

/*[clinic input]
@c_stack_frugal
_codecs.utf_8_decode
    data: Py_buffer
    errors: str(accept={str, NoneType}) = None
    final: bool = False
    /
[clinic start generated code]*/

static PyObject *
_codecs_utf_8_decode_impl(PyObject *module, Py_buffer *data,
                          const char *errors, int final)
/*[clinic end generated code: output=10f74dec8d9bb8bf input=7686b40a5163ebfc]*/
{
    Py_ssize_t consumed = data->len;
    PyObject *decoded = PyUnicode_DecodeUTF8Stateful(data->buf, data->len,
                                                     errors,
                                                     final ? NULL : &consumed);
    return codec_tuple(decoded, consumed);
}

/*[clinic input]
@c_stack_frugal
_codecs.utf_16_decode
    data: Py_buffer
    errors: str(accept={str, NoneType}) = None
    final: bool = False
    /
[clinic start generated code]*/

static PyObject *
_codecs_utf_16_decode_impl(PyObject *module, Py_buffer *data,
                           const char *errors, int final)
/*[clinic end generated code: output=783b442abcbcc2d0 input=af959b533ce7f6c4]*/
{
    int byteorder = 0;
    /* This is overwritten unless final is true. */
    Py_ssize_t consumed = data->len;
    PyObject *decoded = PyUnicode_DecodeUTF16Stateful(data->buf, data->len,
                                                      errors, &byteorder,
                                                      final ? NULL : &consumed);
    return codec_tuple(decoded, consumed);
}

/*[clinic input]
@c_stack_frugal
_codecs.utf_16_le_decode
    data: Py_buffer
    errors: str(accept={str, NoneType}) = None
    final: bool = False
    /
[clinic start generated code]*/

static PyObject *
_codecs_utf_16_le_decode_impl(PyObject *module, Py_buffer *data,
                              const char *errors, int final)
/*[clinic end generated code: output=899b9e6364379dcd input=46f0e679f32e0114]*/
{
    int byteorder = -1;
    /* This is overwritten unless final is true. */
    Py_ssize_t consumed = data->len;
    PyObject *decoded = PyUnicode_DecodeUTF16Stateful(data->buf, data->len,
                                                      errors, &byteorder,
                                                      final ? NULL : &consumed);
    return codec_tuple(decoded, consumed);
}

/*[clinic input]
@c_stack_frugal
_codecs.utf_16_be_decode
    data: Py_buffer
    errors: str(accept={str, NoneType}) = None
    final: bool = False
    /
[clinic start generated code]*/

static PyObject *
_codecs_utf_16_be_decode_impl(PyObject *module, Py_buffer *data,
                              const char *errors, int final)
/*[clinic end generated code: output=49f6465ea07669c8 input=034292772360caa1]*/
{
    int byteorder = 1;
    /* This is overwritten unless final is true. */
    Py_ssize_t consumed = data->len;
    PyObject *decoded = PyUnicode_DecodeUTF16Stateful(data->buf, data->len,
                                                      errors, &byteorder,
                                                      final ? NULL : &consumed);
    return codec_tuple(decoded, consumed);
}

/* This non-standard version also provides access to the byteorder
   parameter of the builtin UTF-16 codec.

   It returns a tuple (unicode, bytesread, byteorder) with byteorder
   being the value in effect at the end of data.

*/
/*[clinic input]
@c_stack_frugal
_codecs.utf_16_ex_decode
    data: Py_buffer
    errors: str(accept={str, NoneType}) = None
    byteorder: int = 0
    final: bool = False
    /
[clinic start generated code]*/

static PyObject *
_codecs_utf_16_ex_decode_impl(PyObject *module, Py_buffer *data,
                              const char *errors, int byteorder, int final)
/*[clinic end generated code: output=0f385f251ecc1988 input=b03fcf155b6d8dfd]*/
{
    /* This is overwritten unless final is true. */
    Py_ssize_t consumed = data->len;

    PyObject *decoded = PyUnicode_DecodeUTF16Stateful(data->buf, data->len,
                                                      errors, &byteorder,
                                                      final ? NULL : &consumed);
    if (decoded == NULL)
        return NULL;
    return Py_BuildValue("Nni", decoded, consumed, byteorder);
}

/*[clinic input]
@c_stack_frugal
_codecs.utf_32_decode
    data: Py_buffer
    errors: str(accept={str, NoneType}) = None
    final: bool = False
    /
[clinic start generated code]*/

static PyObject *
_codecs_utf_32_decode_impl(PyObject *module, Py_buffer *data,
                           const char *errors, int final)
/*[clinic end generated code: output=2fc961807f7b145f input=284f1e554a0afbae]*/
{
    int byteorder = 0;
    /* This is overwritten unless final is true. */
    Py_ssize_t consumed = data->len;
    PyObject *decoded = PyUnicode_DecodeUTF32Stateful(data->buf, data->len,
                                                      errors, &byteorder,
                                                      final ? NULL : &consumed);
    return codec_tuple(decoded, consumed);
}

/*[clinic input]
@c_stack_frugal
_codecs.utf_32_le_decode
    data: Py_buffer
    errors: str(accept={str, NoneType}) = None
    final: bool = False
    /
[clinic start generated code]*/

static PyObject *
_codecs_utf_32_le_decode_impl(PyObject *module, Py_buffer *data,
                              const char *errors, int final)
/*[clinic end generated code: output=ec8f46b67a94f3e6 input=ecf9fab66a8ed93c]*/
{
    int byteorder = -1;
    /* This is overwritten unless final is true. */
    Py_ssize_t consumed = data->len;
    PyObject *decoded = PyUnicode_DecodeUTF32Stateful(data->buf, data->len,
                                                      errors, &byteorder,
                                                      final ? NULL : &consumed);
    return codec_tuple(decoded, consumed);
}

/*[clinic input]
@c_stack_frugal
_codecs.utf_32_be_decode
    data: Py_buffer
    errors: str(accept={str, NoneType}) = None
    final: bool = False
    /
[clinic start generated code]*/

static PyObject *
_codecs_utf_32_be_decode_impl(PyObject *module, Py_buffer *data,
                              const char *errors, int final)
/*[clinic end generated code: output=ff82bae862c92c4e input=03178b208a3842a1]*/
{
    int byteorder = 1;
    /* This is overwritten unless final is true. */
    Py_ssize_t consumed = data->len;
    PyObject *decoded = PyUnicode_DecodeUTF32Stateful(data->buf, data->len,
                                                      errors, &byteorder,
                                                      final ? NULL : &consumed);
    return codec_tuple(decoded, consumed);
}

/* This non-standard version also provides access to the byteorder
   parameter of the builtin UTF-32 codec.

   It returns a tuple (unicode, bytesread, byteorder) with byteorder
   being the value in effect at the end of data.

*/
/*[clinic input]
@c_stack_frugal
_codecs.utf_32_ex_decode
    data: Py_buffer
    errors: str(accept={str, NoneType}) = None
    byteorder: int = 0
    final: bool = False
    /
[clinic start generated code]*/

static PyObject *
_codecs_utf_32_ex_decode_impl(PyObject *module, Py_buffer *data,
                              const char *errors, int byteorder, int final)
/*[clinic end generated code: output=6bfb177dceaf4848 input=4728f60319ee1ca6]*/
{
    Py_ssize_t consumed = data->len;
    PyObject *decoded = PyUnicode_DecodeUTF32Stateful(data->buf, data->len,
                                                      errors, &byteorder,
                                                      final ? NULL : &consumed);
    if (decoded == NULL)
        return NULL;
    return Py_BuildValue("Nni", decoded, consumed, byteorder);
}

/*[clinic input]
@c_stack_frugal
_codecs.unicode_escape_decode
    data: Py_buffer(accept={str, buffer})
    errors: str(accept={str, NoneType}) = None
    final: bool = True
    /
[clinic start generated code]*/

static PyObject *
_codecs_unicode_escape_decode_impl(PyObject *module, Py_buffer *data,
                                   const char *errors, int final)
/*[clinic end generated code: output=b284f97b12c635ee input=758d9c6b835b4b59]*/
{
    Py_ssize_t consumed = data->len;
    PyObject *decoded = _PyUnicode_DecodeUnicodeEscapeStateful(data->buf, data->len,
                                                               errors,
                                                               final ? NULL : &consumed);
    return codec_tuple(decoded, consumed);
}

/*[clinic input]
@c_stack_frugal
_codecs.raw_unicode_escape_decode
    data: Py_buffer(accept={str, buffer})
    errors: str(accept={str, NoneType}) = None
    final: bool = True
    /
[clinic start generated code]*/

static PyObject *
_codecs_raw_unicode_escape_decode_impl(PyObject *module, Py_buffer *data,
                                       const char *errors, int final)
/*[clinic end generated code: output=11dbd96301e2879e input=f8b1b0d2efe477a3]*/
{
    Py_ssize_t consumed = data->len;
    PyObject *decoded = _PyUnicode_DecodeRawUnicodeEscapeStateful(data->buf, data->len,
                                                                  errors,
                                                                  final ? NULL : &consumed);
    return codec_tuple(decoded, consumed);
}

/*[clinic input]
@c_stack_frugal
_codecs.latin_1_decode
    data: Py_buffer
    errors: str(accept={str, NoneType}) = None
    /
[clinic start generated code]*/

static PyObject *
_codecs_latin_1_decode_impl(PyObject *module, Py_buffer *data,
                            const char *errors)
/*[clinic end generated code: output=07f3dfa3f72c7d8f input=305af3ff634c0e47]*/
{
    PyObject *decoded = PyUnicode_DecodeLatin1(data->buf, data->len, errors);
    return codec_tuple(decoded, data->len);
}

/*[clinic input]
@c_stack_frugal
_codecs.ascii_decode
    data: Py_buffer
    errors: str(accept={str, NoneType}) = None
    /
[clinic start generated code]*/

static PyObject *
_codecs_ascii_decode_impl(PyObject *module, Py_buffer *data,
                          const char *errors)
/*[clinic end generated code: output=2627d72058d42429 input=e9bb531e3de03b4f]*/
{
    PyObject *decoded = PyUnicode_DecodeASCII(data->buf, data->len, errors);
    return codec_tuple(decoded, data->len);
}

/*[clinic input]
@c_stack_frugal
_codecs.charmap_decode
    data: Py_buffer
    errors: str(accept={str, NoneType}) = None
    mapping: object = None
    /
[clinic start generated code]*/

static PyObject *
_codecs_charmap_decode_impl(PyObject *module, Py_buffer *data,
                            const char *errors, PyObject *mapping)
/*[clinic end generated code: output=2c335b09778cf895 input=90f7dfd56405fc55]*/
{
    PyObject *decoded;

    if (mapping == Py_None)
        mapping = NULL;

    decoded = PyUnicode_DecodeCharmap(data->buf, data->len, mapping, errors);
    return codec_tuple(decoded, data->len);
}

#ifdef MS_WINDOWS

/*[clinic input]
@c_stack_frugal
_codecs.mbcs_decode
    data: Py_buffer
    errors: str(accept={str, NoneType}) = None
    final: bool = False
    /
[clinic start generated code]*/

static PyObject *
_codecs_mbcs_decode_impl(PyObject *module, Py_buffer *data,
                         const char *errors, int final)
/*[clinic end generated code: output=39b65b8598938c4b input=4be12e2878b9888d]*/
{
    Py_ssize_t consumed = data->len;
    PyObject *decoded = PyUnicode_DecodeMBCSStateful(data->buf, data->len,
            errors, final ? NULL : &consumed);
    return codec_tuple(decoded, consumed);
}

/*[clinic input]
@c_stack_frugal
_codecs.oem_decode
    data: Py_buffer
    errors: str(accept={str, NoneType}) = None
    final: bool = False
    /
[clinic start generated code]*/

static PyObject *
_codecs_oem_decode_impl(PyObject *module, Py_buffer *data,
                        const char *errors, int final)
/*[clinic end generated code: output=da1617612f3fcad8 input=cb644e385e013f0d]*/
{
    Py_ssize_t consumed = data->len;
    PyObject *decoded = PyUnicode_DecodeCodePageStateful(CP_OEMCP,
        data->buf, data->len, errors, final ? NULL : &consumed);
    return codec_tuple(decoded, consumed);
}

/*[clinic input]
@c_stack_frugal
_codecs.code_page_decode
    codepage: int
    data: Py_buffer
    errors: str(accept={str, NoneType}) = None
    final: bool = False
    /
[clinic start generated code]*/

static PyObject *
_codecs_code_page_decode_impl(PyObject *module, int codepage,
                              Py_buffer *data, const char *errors, int final)
/*[clinic end generated code: output=53008ea967da3fff input=1603f486b06d7776]*/
{
    Py_ssize_t consumed = data->len;
    PyObject *decoded = PyUnicode_DecodeCodePageStateful(codepage,
                                                         data->buf, data->len,
                                                         errors,
                                                         final ? NULL : &consumed);
    return codec_tuple(decoded, consumed);
}

#endif /* MS_WINDOWS */

/* --- Encoder ------------------------------------------------------------ */

/*[clinic input]
@c_stack_frugal
_codecs.readbuffer_encode
    data: Py_buffer(accept={str, buffer})
    errors: str(accept={str, NoneType}) = None
    /
[clinic start generated code]*/

static PyObject *
_codecs_readbuffer_encode_impl(PyObject *module, Py_buffer *data,
                               const char *errors)
/*[clinic end generated code: output=c645ea7cdb3d6e86 input=3f8755ef2acc83ed]*/
{
    PyObject *result = PyBytes_FromStringAndSize(data->buf, data->len);
    return codec_tuple(result, data->len);
}

/*[clinic input]
@c_stack_frugal
_codecs.utf_7_encode
    str: unicode
    errors: str(accept={str, NoneType}) = None
    /
[clinic start generated code]*/

static PyObject *
_codecs_utf_7_encode_impl(PyObject *module, PyObject *str,
                          const char *errors)
/*[clinic end generated code: output=0feda21ffc921bc8 input=64a71d8eab425735]*/
{
    return codec_tuple(_PyUnicode_EncodeUTF7(str, 0, 0, errors),
                       PyUnicode_GET_LENGTH(str));
}

/*[clinic input]
@c_stack_frugal
_codecs.utf_8_encode
    str: unicode
    errors: str(accept={str, NoneType}) = None
    /
[clinic start generated code]*/

static PyObject *
_codecs_utf_8_encode_impl(PyObject *module, PyObject *str,
                          const char *errors)
/*[clinic end generated code: output=02bf47332b9c796c input=dd4afee1ae5330dc]*/
{
    return codec_tuple(_PyUnicode_AsUTF8String(str, errors),
                       PyUnicode_GET_LENGTH(str));
}

/* This version provides access to the byteorder parameter of the
   builtin UTF-16 codecs as optional third argument. It defaults to 0
   which means: use the native byte order and prepend the data with a
   BOM mark.

*/

/*[clinic input]
@c_stack_frugal
_codecs.utf_16_encode
    str: unicode
    errors: str(accept={str, NoneType}) = None
    byteorder: int = 0
    /
[clinic start generated code]*/

static PyObject *
_codecs_utf_16_encode_impl(PyObject *module, PyObject *str,
                           const char *errors, int byteorder)
/*[clinic end generated code: output=c654e13efa2e64e4 input=690687b0339213e1]*/
{
    return codec_tuple(_PyUnicode_EncodeUTF16(str, errors, byteorder),
                       PyUnicode_GET_LENGTH(str));
}

/*[clinic input]
@c_stack_frugal
_codecs.utf_16_le_encode
    str: unicode
    errors: str(accept={str, NoneType}) = None
    /
[clinic start generated code]*/

static PyObject *
_codecs_utf_16_le_encode_impl(PyObject *module, PyObject *str,
                              const char *errors)
/*[clinic end generated code: output=431b01e55f2d4995 input=405565f3c4fc612f]*/
{
    return codec_tuple(_PyUnicode_EncodeUTF16(str, errors, -1),
                       PyUnicode_GET_LENGTH(str));
}

/*[clinic input]
@c_stack_frugal
_codecs.utf_16_be_encode
    str: unicode
    errors: str(accept={str, NoneType}) = None
    /
[clinic start generated code]*/

static PyObject *
_codecs_utf_16_be_encode_impl(PyObject *module, PyObject *str,
                              const char *errors)
/*[clinic end generated code: output=96886a6fd54dcae3 input=96f7e92fc2383620]*/
{
    return codec_tuple(_PyUnicode_EncodeUTF16(str, errors, +1),
                       PyUnicode_GET_LENGTH(str));
}

/* This version provides access to the byteorder parameter of the
   builtin UTF-32 codecs as optional third argument. It defaults to 0
   which means: use the native byte order and prepend the data with a
   BOM mark.

*/

/*[clinic input]
@c_stack_frugal
_codecs.utf_32_encode
    str: unicode
    errors: str(accept={str, NoneType}) = None
    byteorder: int = 0
    /
[clinic start generated code]*/

static PyObject *
_codecs_utf_32_encode_impl(PyObject *module, PyObject *str,
                           const char *errors, int byteorder)
/*[clinic end generated code: output=5c760da0c09a8b83 input=384161fe37d73809]*/
{
    return codec_tuple(_PyUnicode_EncodeUTF32(str, errors, byteorder),
                       PyUnicode_GET_LENGTH(str));
}

/*[clinic input]
@c_stack_frugal
_codecs.utf_32_le_encode
    str: unicode
    errors: str(accept={str, NoneType}) = None
    /
[clinic start generated code]*/

static PyObject *
_codecs_utf_32_le_encode_impl(PyObject *module, PyObject *str,
                              const char *errors)
/*[clinic end generated code: output=b65cd176de8e36d6 input=c0da8141c1f524e3]*/
{
    return codec_tuple(_PyUnicode_EncodeUTF32(str, errors, -1),
                       PyUnicode_GET_LENGTH(str));
}

/*[clinic input]
@c_stack_frugal
_codecs.utf_32_be_encode
    str: unicode
    errors: str(accept={str, NoneType}) = None
    /
[clinic start generated code]*/

static PyObject *
_codecs_utf_32_be_encode_impl(PyObject *module, PyObject *str,
                              const char *errors)
/*[clinic end generated code: output=1d9e71a9358709e9 input=a44fd369735c2510]*/
{
    return codec_tuple(_PyUnicode_EncodeUTF32(str, errors, +1),
                       PyUnicode_GET_LENGTH(str));
}

/*[clinic input]
@c_stack_frugal
_codecs.unicode_escape_encode
    str: unicode
    errors: str(accept={str, NoneType}) = None
    /
[clinic start generated code]*/

static PyObject *
_codecs_unicode_escape_encode_impl(PyObject *module, PyObject *str,
                                   const char *errors)
/*[clinic end generated code: output=66271b30bc4f7a3c input=735f2e0d659c4ffd]*/
{
    return codec_tuple(PyUnicode_AsUnicodeEscapeString(str),
                       PyUnicode_GET_LENGTH(str));
}

/*[clinic input]
@c_stack_frugal
_codecs.raw_unicode_escape_encode
    str: unicode
    errors: str(accept={str, NoneType}) = None
    /
[clinic start generated code]*/

static PyObject *
_codecs_raw_unicode_escape_encode_impl(PyObject *module, PyObject *str,
                                       const char *errors)
/*[clinic end generated code: output=a66a806ed01c830a input=d339d6ceb29e9afa]*/
{
    return codec_tuple(PyUnicode_AsRawUnicodeEscapeString(str),
                       PyUnicode_GET_LENGTH(str));
}

/*[clinic input]
@c_stack_frugal
_codecs.latin_1_encode
    str: unicode
    errors: str(accept={str, NoneType}) = None
    /
[clinic start generated code]*/

static PyObject *
_codecs_latin_1_encode_impl(PyObject *module, PyObject *str,
                            const char *errors)
/*[clinic end generated code: output=2c28c83a27884e08 input=1ec918e2e9b81ab1]*/
{
    return codec_tuple(_PyUnicode_AsLatin1String(str, errors),
                       PyUnicode_GET_LENGTH(str));
}

/*[clinic input]
@c_stack_frugal
_codecs.ascii_encode
    str: unicode
    errors: str(accept={str, NoneType}) = None
    /
[clinic start generated code]*/

static PyObject *
_codecs_ascii_encode_impl(PyObject *module, PyObject *str,
                          const char *errors)
/*[clinic end generated code: output=b5e035182d33befc input=5062e46ab24e89bf]*/
{
    return codec_tuple(_PyUnicode_AsASCIIString(str, errors),
                       PyUnicode_GET_LENGTH(str));
}

/*[clinic input]
@c_stack_frugal
_codecs.charmap_encode
    str: unicode
    errors: str(accept={str, NoneType}) = None
    mapping: object = None
    /
[clinic start generated code]*/

static PyObject *
_codecs_charmap_encode_impl(PyObject *module, PyObject *str,
                            const char *errors, PyObject *mapping)
/*[clinic end generated code: output=047476f48495a9e9 input=4a6291e3992ff875]*/
{
    if (mapping == Py_None)
        mapping = NULL;

    return codec_tuple(_PyUnicode_EncodeCharmap(str, mapping, errors),
                       PyUnicode_GET_LENGTH(str));
}

/*[clinic input]
@c_stack_frugal
_codecs.charmap_build
    map: unicode
    /
[clinic start generated code]*/

static PyObject *
_codecs_charmap_build_impl(PyObject *module, PyObject *map)
/*[clinic end generated code: output=bb073c27031db9ac input=71fd895712becb78]*/
{
    return PyUnicode_BuildEncodingMap(map);
}

#ifdef MS_WINDOWS

/*[clinic input]
@c_stack_frugal
_codecs.mbcs_encode
    str: unicode
    errors: str(accept={str, NoneType}) = None
    /
[clinic start generated code]*/

static PyObject *
_codecs_mbcs_encode_impl(PyObject *module, PyObject *str, const char *errors)
/*[clinic end generated code: output=76e2e170c966c080 input=f4e91fbab67dbade]*/
{
    return codec_tuple(PyUnicode_EncodeCodePage(CP_ACP, str, errors),
                       PyUnicode_GET_LENGTH(str));
}

/*[clinic input]
@c_stack_frugal
_codecs.oem_encode
    str: unicode
    errors: str(accept={str, NoneType}) = None
    /
[clinic start generated code]*/

static PyObject *
_codecs_oem_encode_impl(PyObject *module, PyObject *str, const char *errors)
/*[clinic end generated code: output=65d5982c737de649 input=91b70470f76eb241]*/
{
    return codec_tuple(PyUnicode_EncodeCodePage(CP_OEMCP, str, errors),
        PyUnicode_GET_LENGTH(str));
}

/*[clinic input]
@c_stack_frugal
_codecs.code_page_encode
    code_page: int
    str: unicode
    errors: str(accept={str, NoneType}) = None
    /
[clinic start generated code]*/

static PyObject *
_codecs_code_page_encode_impl(PyObject *module, int code_page, PyObject *str,
                              const char *errors)
/*[clinic end generated code: output=45673f6085657a9e input=8ab28c8a7f821296]*/
{
    return codec_tuple(PyUnicode_EncodeCodePage(code_page, str, errors),
                       PyUnicode_GET_LENGTH(str));
}

#endif /* MS_WINDOWS */

/* --- Error handler registry --------------------------------------------- */

/*[clinic input]
@c_stack_frugal
_codecs.register_error
    errors: str
    handler: object
    /

Register the specified error handler under the name errors.

handler must be a callable object, that will be called with an exception
instance containing information about the location of the encoding/decoding
error and must return a (replacement, new position) tuple.
[clinic start generated code]*/

static PyObject *
_codecs_register_error_impl(PyObject *module, const char *errors,
                            PyObject *handler)
/*[clinic end generated code: output=fa2f7d1879b3067d input=b48fe9f210c53d05]*/
{
    if (PyCodec_RegisterError(errors, handler))
        return NULL;
    Py_RETURN_NONE;
}

/*[clinic input]
@c_stack_frugal
_codecs._unregister_error -> bool
    errors: str
    /

Un-register the specified error handler for the error handling `errors'.

Only custom error handlers can be un-registered. An exception is raised
if the error handling is a built-in one (e.g., 'strict'), or if an error
occurs.

Otherwise, this returns True if a custom handler has been successfully
un-registered, and False if no custom handler for the specified error
handling exists.

[clinic start generated code]*/

static int
_codecs__unregister_error_impl(PyObject *module, const char *errors)
/*[clinic end generated code: output=28c22be667465503 input=228c4a0708bf318f]*/
{
    return _PyCodec_UnregisterError(errors);
}

/*[clinic input]
@c_stack_frugal
_codecs.lookup_error
    name: str
    /

lookup_error(errors) -> handler

Return the error handler for the specified error handling name or raise a
LookupError, if no handler exists under this name.
[clinic start generated code]*/

static PyObject *
_codecs_lookup_error_impl(PyObject *module, const char *name)
/*[clinic end generated code: output=087f05dc0c9a98cc input=f98cd8dad379feea]*/
{
    return PyCodec_LookupError(name);
}

/* --- Module API --------------------------------------------------------- */

static PyMethodDef _codecs_functions[] = {
    _CODECS_REGISTER_METHODDEF
    _CODECS_UNREGISTER_METHODDEF
    _CODECS_LOOKUP_METHODDEF
    _CODECS_ENCODE_METHODDEF
    _CODECS_DECODE_METHODDEF
    _CODECS_ESCAPE_ENCODE_METHODDEF
    _CODECS_ESCAPE_DECODE_METHODDEF
    _CODECS_UTF_8_ENCODE_METHODDEF
    _CODECS_UTF_8_DECODE_METHODDEF
    _CODECS_UTF_7_ENCODE_METHODDEF
    _CODECS_UTF_7_DECODE_METHODDEF
    _CODECS_UTF_16_ENCODE_METHODDEF
    _CODECS_UTF_16_LE_ENCODE_METHODDEF
    _CODECS_UTF_16_BE_ENCODE_METHODDEF
    _CODECS_UTF_16_DECODE_METHODDEF
    _CODECS_UTF_16_LE_DECODE_METHODDEF
    _CODECS_UTF_16_BE_DECODE_METHODDEF
    _CODECS_UTF_16_EX_DECODE_METHODDEF
    _CODECS_UTF_32_ENCODE_METHODDEF
    _CODECS_UTF_32_LE_ENCODE_METHODDEF
    _CODECS_UTF_32_BE_ENCODE_METHODDEF
    _CODECS_UTF_32_DECODE_METHODDEF
    _CODECS_UTF_32_LE_DECODE_METHODDEF
    _CODECS_UTF_32_BE_DECODE_METHODDEF
    _CODECS_UTF_32_EX_DECODE_METHODDEF
    _CODECS_UNICODE_ESCAPE_ENCODE_METHODDEF
    _CODECS_UNICODE_ESCAPE_DECODE_METHODDEF
    _CODECS_RAW_UNICODE_ESCAPE_ENCODE_METHODDEF
    _CODECS_RAW_UNICODE_ESCAPE_DECODE_METHODDEF
    _CODECS_LATIN_1_ENCODE_METHODDEF
    _CODECS_LATIN_1_DECODE_METHODDEF
    _CODECS_ASCII_ENCODE_METHODDEF
    _CODECS_ASCII_DECODE_METHODDEF
    _CODECS_CHARMAP_ENCODE_METHODDEF
    _CODECS_CHARMAP_DECODE_METHODDEF
    _CODECS_CHARMAP_BUILD_METHODDEF
    _CODECS_READBUFFER_ENCODE_METHODDEF
    _CODECS_MBCS_ENCODE_METHODDEF
    _CODECS_MBCS_DECODE_METHODDEF
    _CODECS_OEM_ENCODE_METHODDEF
    _CODECS_OEM_DECODE_METHODDEF
    _CODECS_CODE_PAGE_ENCODE_METHODDEF
    _CODECS_CODE_PAGE_DECODE_METHODDEF
    _CODECS_REGISTER_ERROR_METHODDEF
    _CODECS__UNREGISTER_ERROR_METHODDEF
    _CODECS_LOOKUP_ERROR_METHODDEF
    {NULL, NULL}                /* sentinel */
};

static PyModuleDef_Slot _codecs_slots[] = {
    {Py_mod_multiple_interpreters, Py_MOD_PER_INTERPRETER_GIL_SUPPORTED},
    {Py_mod_gil, Py_MOD_GIL_NOT_USED},
    {0, NULL}
};

static struct PyModuleDef codecsmodule = {
        PyModuleDef_HEAD_INIT,
        "_codecs",
        NULL,
        0,
        _codecs_functions,
        _codecs_slots,
        NULL,
        NULL,
        NULL
};

PyMODINIT_FUNC
PyInit__codecs(void)
{
    return PyModuleDef_Init(&codecsmodule);
}
