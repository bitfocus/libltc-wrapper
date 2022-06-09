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

void destroy_ltc_object(napi_env env, void *data, void *hint)
{
	struct ltc_decoder_object *obj = (struct ltc_decoder_object *)data;

	if (obj->decoder != NULL) {
		ltc_decoder_free(obj->decoder);
		obj->decoder = NULL;
	}

	free(obj);
}

// create_ltc_decoder(apv, queue_size, audio_format)
static napi_value create_ltc_decoder(napi_env env, napi_callback_info info)
{
	napi_value result;
	napi_status status;

	size_t argc = 3;
	napi_value args[3];
	napi_valuetype type;
	struct ltc_decoder_object *obj;

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

	obj = (struct ltc_decoder_object *)calloc(sizeof(struct ltc_decoder_object), 1);
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

	if (strncmp(format, "u8", format_len) == 0) {
		obj->sound_format = LTC_SOUND_FORMAT_U8;
	}
	else if (strncmp(format, "s16", format_len) == 0) {
		obj->sound_format = LTC_SOUND_FORMAT_S16;
	}
	else if (strncmp(format, "u16", format_len) == 0) {
		obj->sound_format = LTC_SOUND_FORMAT_U16;
	}
	else if (strncmp(format, "float", format_len) == 0) {
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

	status = napi_create_external(env, obj, destroy_ltc_object, NULL, &result);
	NAPI_STATUS_RETURN("Failed to create external");

	return result;
}

static napi_value write_audio(napi_env env, napi_callback_info info)
{
	napi_status status;
	struct ltc_decoder_object *obj;

	size_t argc = 2;
	napi_value args[2];
	napi_valuetype type;

	status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
	NAPI_STATUS_RETURN("Failed to parse arguments")

	if (argc < 2) {
		NAPI_ERROR_RETURN("Wrong number of arguments");
	}

	status = napi_typeof(env, args[0], &type);
	NAPI_STATUS_RETURN("Error fetching type of argument 1");

	if (type != napi_external) {
		NAPI_ERROR_RETURN("Expected argument 1 to be an external");
	}

	bool is_buffer;
	status = napi_is_buffer(env, args[1], &is_buffer);
	NAPI_STATUS_RETURN("Error fetching type of argument 2");

	if (!is_buffer) {
		NAPI_ERROR_RETURN("Expected argument 2 to be a buffer");
	}

	status = napi_get_value_external(env, args[0], (void **)&obj);
	NAPI_STATUS_RETURN("Failed to get external");

	uint8_t *buffer_data;
	size_t buffer_size;
	
	napi_get_buffer_info(env, args[1], (void **)&buffer_data, &buffer_size);

	switch (obj->sound_format) {
		case LTC_SOUND_FORMAT_U8:
			ltc_decoder_write(obj->decoder, (ltcsnd_sample_t *)buffer_data, buffer_size, obj->position);
			obj->position += buffer_size;
			break;
		case LTC_SOUND_FORMAT_U16:
			if (buffer_size % 2 != 0) {
				NAPI_ERROR_RETURN("Buffer size must be a multiple of 2");
			}

			ltc_decoder_write_u16(obj->decoder, (unsigned short *)buffer_data, buffer_size / 2, obj->position);
			obj->position += buffer_size / 2;
			break;
		case LTC_SOUND_FORMAT_S16:
			if (buffer_size % 2 != 0) {
				NAPI_ERROR_RETURN("Buffer size must be a multiple of 2");
			}

			ltc_decoder_write_s16(obj->decoder, (short *)buffer_data, buffer_size / 2, obj->position);
			obj->position += buffer_size / 2;
			break;
		case LTC_SOUND_FORMAT_FL:
			if (buffer_size % 4 != 0) {
				NAPI_ERROR_RETURN("Buffer size must be a multiple of 4");
			}

			ltc_decoder_write_float(obj->decoder, (float *)buffer_data, buffer_size / 4, obj->position);
			obj->position += buffer_size / 4;
			break;
		default:
			NAPI_ERROR_RETURN("Invalid sound format");
	}

	napi_value undefined;
	status = napi_get_undefined(env, &undefined);
	NAPI_STATUS_RETURN("Failed to get undefined")

	return undefined;
}

static napi_value read_frame(napi_env env, napi_callback_info info)
{
	napi_status status;
	struct ltc_decoder_object *obj;

	size_t argc = 1;
	napi_value args[1];
	napi_valuetype type;

	status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
	NAPI_STATUS_RETURN("Failed to parse arguments")

	if (argc < 1) {
		NAPI_ERROR_RETURN("Wrong number of arguments");
	}

	status = napi_typeof(env, args[0], &type);
	NAPI_STATUS_RETURN("Error fetching type of argument 1");

	if (type != napi_external) {
		NAPI_ERROR_RETURN("Expected argument 1 to be an external");
	}

	status = napi_get_value_external(env, args[0], (void **)&obj);
	NAPI_STATUS_RETURN("Failed to get external");

	int result = ltc_decoder_read(obj->decoder, &obj->frame);

	if (result == 0) {
		// No frames yet
		napi_value undefined;
		status = napi_get_undefined(env, &undefined);
		NAPI_STATUS_RETURN("Failed to get undefined")

		return undefined;
	} else {
		// Return frame
		SMPTETimecode stime;
		ltc_frame_to_time(&stime, &obj->frame.ltc, 1);

		napi_value result;
		status = napi_create_object(env, &result);

		napi_value ltc_df;
		status = napi_get_boolean(env, obj->frame.ltc.dfbit != 0, &ltc_df);
		NAPI_STATUS_RETURN("Failed to get boolean")
		status = napi_set_named_property(env, result, "dropped_frame", ltc_df);

		napi_value days;
		status = napi_create_int32(env, stime.days, &days);
		NAPI_STATUS_RETURN("Failed to create int32")
		status = napi_set_named_property(env, result, "days", days);

		napi_value months;
		status = napi_create_int32(env, stime.months, &months);
		NAPI_STATUS_RETURN("Failed to create int32")
		status = napi_set_named_property(env, result, "months", months);

		napi_value years;
		status = napi_create_int32(env, stime.years, &years);
		NAPI_STATUS_RETURN("Failed to create int32")
		status = napi_set_named_property(env, result, "years", years);

		napi_value hours;
		status = napi_create_int32(env, stime.hours, &hours);
		NAPI_STATUS_RETURN("Failed to create int32")
		status = napi_set_named_property(env, result, "hours", hours);

		napi_value minutes;
		status = napi_create_int32(env, stime.mins, &minutes);
		NAPI_STATUS_RETURN("Failed to create int32")
		status = napi_set_named_property(env, result, "minutes", minutes);

		napi_value seconds;
		status = napi_create_int32(env, stime.secs, &seconds);
		NAPI_STATUS_RETURN("Failed to create int32")
		status = napi_set_named_property(env, result, "seconds", seconds);

		napi_value frames;
		status = napi_create_int32(env, stime.frame, &frames);
		NAPI_STATUS_RETURN("Failed to create int32")
		status = napi_set_named_property(env, result, "frames", frames);

		napi_value offset_start;
		status = napi_create_double(env, obj->frame.off_start, &offset_start);
		NAPI_STATUS_RETURN("Failed to create double")
		status = napi_set_named_property(env, result, "offset_start", offset_start);

		napi_value reverse;
		status = napi_get_boolean(env, obj->frame.reverse != 0, &reverse);
		NAPI_STATUS_RETURN("Failed to get boolean")
		status = napi_set_named_property(env, result, "reverse", reverse);

		napi_value volume;
		status = napi_create_double(env, obj->frame.volume, &volume);
		NAPI_STATUS_RETURN("Failed to create double")
		status = napi_set_named_property(env, result, "volume", volume);

		napi_value timezone;
		status = napi_create_string_utf8(env, stime.timezone, 5, &timezone);
		NAPI_STATUS_RETURN("Failed to create string")
		status = napi_set_named_property(env, result, "timezone", timezone);

		return result;
	}
}

napi_value Init(napi_env env, napi_value exports)
{

	napi_value create_ltc_decoder_function;
	napi_value write_audio_function;
	napi_value read_frame_function;

	NAPI_CALL(env, napi_create_function(env, "createLTCDecoder", NAPI_AUTO_LENGTH, create_ltc_decoder, NULL, &create_ltc_decoder_function));
	NAPI_CALL(env, napi_set_named_property(env, exports, "createLTCDecoder", create_ltc_decoder_function));

	NAPI_CALL(env, napi_create_function(env, "writeAudio", NAPI_AUTO_LENGTH, write_audio, NULL, &write_audio_function));
	NAPI_CALL(env, napi_set_named_property(env, exports, "writeAudio", write_audio_function));


	NAPI_CALL(env, napi_create_function(env, "readFrame", NAPI_AUTO_LENGTH, read_frame, NULL, &read_frame_function));
	NAPI_CALL(env, napi_set_named_property(env, exports, "readFrame", read_frame_function));

	return exports;
}

NAPI_MODULE(nodencl, Init)
