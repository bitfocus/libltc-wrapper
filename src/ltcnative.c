#define NAPI_VERSION 4
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <ltc.h>

#include <node_api.h>
#include <js_native_api.h>
#include "ltcnative.h"

void destroyLtcObject(napi_env env, void *data, void *hint)
{
	struct ltcobject *obj = (struct ltcobject *)data;

	if (obj->decoder != NULL)
	{
		ltc_decoder_free(obj->decoder);
	}

	free(obj);
}

// create_ltc_object(apv, queue_size, audio_format)
static napi_value create_ltc_object(napi_env env, napi_callback_info info)
{
	napi_value result;
	napi_status status;

	size_t argc = 3;
	napi_value args[3];
	napi_valuetype type;
	struct ltcobject *obj;

	char *format;
	size_t format_len;

	status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
	NAPI_STATUS_RETURN("Failed to parse arguments")

	if (argc < 3) {
		NAPI_ERROR_RETURN("Wrong number of arguments");
	}

	status = napi_typeof(env, args[0], &type);
	NAPI_STATUS_RETURN("Error fetching type of argument 1");
	if (type != napi_number) {
		NAPI_ERROR_RETURN("Expected argument 1 to be a number");
	}

	status = napi_typeof(env, args[1], &type);
	NAPI_STATUS_RETURN("Error fetching type of argument 2");
	if (type != napi_number) {
		NAPI_ERROR_RETURN("Expected argument 2 to be a number");
	}

	status = napi_typeof(env, args[2], &type);
	NAPI_STATUS_RETURN("Error fetching type of argument 3");
	if (type != napi_string) {
		NAPI_ERROR_RETURN("Expected argument 3 to be a string");
	}

	obj = (struct ltcobject *)malloc(sizeof(struct ltcobject));
	if (obj == NULL) {
		NAPI_ERROR_RETURN("Error allocating memory");
	}

	napi_get_value_int32(env, args[0], &obj->apv);
	napi_get_value_int32(env, args[1], &obj->queue_size);

	status = napi_get_value_string_utf8(env, args[2], NULL, 0, &format_len);
	if (status != napi_ok) {
		free(obj);
		NAPI_ERROR_RETURN("Error fetching length of argument 3");
	}

	format = (char *)calloc(sizeof(char), format_len + 1);
	if (format == NULL) {
		free(obj);
		NAPI_ERROR_RETURN("Failed to allocate memory");
	}

	status = napi_get_value_string_utf8(env, args[2], format, format_len + 1, &format_len);
	if (status != napi_ok) {
		free(format);
		free(obj);
		NAPI_ERROR_RETURN("Failed to get string")
	}

	if (strncmp(format, "u8", format_len)) {
		obj->sound_format = LTC_SOUND_FORMAT_U8;
	}
	else if (strncmp(format, "s16", format_len)) {
		obj->sound_format = LTC_SOUND_FORMAT_S16;
	}
	else if (strncmp(format, "u16", format_len)) {
		obj->sound_format = LTC_SOUND_FORMAT_U16;
	}
	else if (strncmp(format, "float", format_len)) {
		obj->sound_format = LTC_SOUND_FORMAT_FL;
	}
	else {
		free(format);
		free(obj);
		NAPI_ERROR_RETURN("Invalid sound format");
	}

	obj->decoder = ltc_decoder_create(obj->apv, obj->queue_size);
	if (obj->decoder == NULL) {
		free(format);
		free(obj);
		NAPI_ERROR_RETURN("Failed to create decoder");
	}

	status = napi_create_external(env, obj, destroyLtcObject, NULL, &result);
	NAPI_STATUS_RETURN("Failed to create external");

	return result;
}

napi_value Init(napi_env env, napi_value exports)
{

	napi_value create_ltc_object_function;

	NAPI_CALL(env, napi_create_function(env, "createLTCObject", NAPI_AUTO_LENGTH, create_ltc_object, NULL, &create_ltc_object_function));
	NAPI_CALL(env, napi_set_named_property(env, exports, "createLTCObject", create_ltc_object_function));

	return exports;
}

NAPI_MODULE(nodencl, Init)
