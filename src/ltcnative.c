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

static napi_value create_ltc_object(napi_env env, napi_callback_info info)
{
	napi_value result;
	napi_status status;

	size_t argc = 3;
	napi_value args[3];
	napi_valuetype type;
	struct ltcobject *obj;

	char *mode;
	size_t mode_len;

	status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
	NAPI_STATUS_RETURN("Failed to parse arguments")

	if (argc < 2) {
		NAPI_ERROR_RETURN("Wrong number of arguments");
	}

	status = napi_typeof(env, args[0], &type);
	NAPI_STATUS_RETURN("Error fetching type of argument 1");
	if (type != napi_string) {
		NAPI_ERROR_RETURN("Expected argument 1 to be a string");
	}

	status = napi_typeof(env, args[1], &type);
	NAPI_STATUS_RETURN("Error fetching type of argument 2");
	if (type != napi_string) {
		NAPI_ERROR_RETURN("Expected argument 2 to be a string");
	}

	// temp
	status = napi_create_external(env, obj, destroyLtcObject, NULL, &result);
	NAPI_STATUS_RETURN("Failed to create external");


	return result;
}

napi_value Init(napi_env env, napi_value exports)
{

	napi_value create_interface_object_function;

	NAPI_CALL(env, napi_create_function(env, "createInterfaceObject", NAPI_AUTO_LENGTH, create_ltc_object, NULL, &create_interface_object_function));
	NAPI_CALL(env, napi_set_named_property(env, exports, "createInterfaceObject", create_ltc_object));

	return exports;
}

NAPI_MODULE(nodencl, Init)
