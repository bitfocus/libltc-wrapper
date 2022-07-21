#define NAPI_VERSION 4
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <errno.h>
#include <ltc.h>

#include <node_api.h>
#include <js_native_api.h>
#include "ltcnative.h"

static void destroy_ltc_encoder_object(napi_env env, void *data, void *hint)
{
	struct ltc_encoder_object *obj = (struct ltc_encoder_object *)data;

	if (obj->encoder != NULL) {
		ltc_encoder_free(obj->encoder);
		obj->encoder = NULL;
	}

	free(obj);
}

// create_ltc_encoder(sample_rate, fps, flags)
static napi_value create_ltc_encoder(napi_env env, napi_callback_info info)
{
	napi_value result;
	napi_status status;

	size_t argc = 3;
	napi_value args[3];
	napi_valuetype type;
	struct ltc_encoder_object *obj;

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

	obj = (struct ltc_encoder_object *)calloc(sizeof(struct ltc_encoder_object), 1);
	if (obj == NULL) {
		NAPI_ERROR_RETURN("Error allocating memory");
	}

	status = napi_typeof(env, args[2], &type);
	NAPI_STATUS_RETURN("Error fetching type of argument 3");

	status = napi_get_value_double(env, args[0], &obj->sample_rate);
	NAPI_STATUS_RETURN("Error fetching value of argument 1");

	status = napi_get_value_double(env, args[1], &obj->fps);
	NAPI_STATUS_RETURN("Error fetching value of argument 2");

	status = napi_get_value_int32(env, args[2], &obj->flags);
	NAPI_STATUS_RETURN("Error fetching value of argument 3");

	obj->encoder = ltc_encoder_create(obj->sample_rate, obj->fps, obj->fps == 25 ? LTC_TV_625_50 : LTC_TV_525_60, obj->flags);

	status = napi_create_external(env, obj, destroy_ltc_encoder_object, NULL, &result);
	NAPI_STATUS_RETURN("Failed to create external");

	return result;
}

static napi_value encoder_set_volume(napi_env env, napi_callback_info info)
{
	napi_status status;

	size_t argc = 2;
	napi_value args[2];
	napi_valuetype type;
	struct ltc_encoder_object *obj;

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

	status = napi_typeof(env, args[1], &type);
	NAPI_STATUS_RETURN("Error fetching type of argument 2");
	if (type != napi_number) {
		NAPI_ERROR_RETURN("Expected argument 2 to be a number");
	}

	status = napi_get_value_external(env, args[0], (void **)&obj);
	NAPI_STATUS_RETURN("Error fetching value of argument 1");

	double dbFS;
	status = napi_get_value_double(env, args[1], &dbFS);
	NAPI_STATUS_RETURN("Error fetching value of argument 2");

	int result = ltc_encoder_set_volume(obj->encoder, dbFS);
	if (result == -1) {
		NAPI_ERROR_RETURN("Invalid volume level, should be <= 0 dBFS");
	}

	return NULL;
}

static napi_value encoder_set_filter(napi_env env, napi_callback_info info)
{
	napi_status status;

	size_t argc = 2;
	napi_value args[2];
	napi_valuetype type;
	struct ltc_encoder_object *obj;

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

	status = napi_typeof(env, args[1], &type);
	NAPI_STATUS_RETURN("Error fetching type of argument 2");
	if (type != napi_number) {
		NAPI_ERROR_RETURN("Expected argument 2 to be a number");
	}

	status = napi_get_value_external(env, args[0], (void **)&obj);
	NAPI_STATUS_RETURN("Error fetching value of argument 1");

	double rise_time;
	status = napi_get_value_double(env, args[1], &rise_time);
	NAPI_STATUS_RETURN("Error fetching value of argument 2");

	ltc_encoder_set_filter(obj->encoder, rise_time);

	return NULL;
}

static napi_value encoder_set_timecode(napi_env env, napi_callback_info info)
{
	napi_status status;

	size_t argc = 2;
	napi_value args[2];
	napi_valuetype type;
	struct ltc_encoder_object *obj;

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

	status = napi_typeof(env, args[1], &type);
	NAPI_STATUS_RETURN("Error fetching type of argument 2");
	if (type != napi_object) {
		NAPI_ERROR_RETURN("Expected argument 2 to be a object");
	}

	status = napi_get_value_external(env, args[0], (void **)&obj);
	NAPI_STATUS_RETURN("Error fetching value of argument 1");

	SMPTETimecode timecode;
	napi_value value;
	int32_t intvalue;
	size_t stringlength;

	NAPI_GETOBJPARAM_NUMBER_ORRETURN("years", timecode.years);
	NAPI_GETOBJPARAM_NUMBER_ORRETURN("months", timecode.months);
	NAPI_GETOBJPARAM_NUMBER_ORRETURN("days", timecode.days);

	NAPI_GETOBJPARAM_NUMBER_ORRETURN("hours", timecode.hours);
	NAPI_GETOBJPARAM_NUMBER_ORRETURN("minutes", timecode.mins);
	NAPI_GETOBJPARAM_NUMBER_ORRETURN("seconds", timecode.secs);

	NAPI_GETOBJPARAM_NUMBER_ORRETURN("frame", timecode.frame);

	NAPI_GETOBJPARAM_STRING_ORRETURN("timezone", (char *)&timecode.timezone, 6);
	if (stringlength != 5) {
		NAPI_ERROR_RETURN("Timezone string in timecode object has invalid length");
	}

	ltc_encoder_set_timecode(obj->encoder, &timecode);

	return NULL;
}

static napi_value encoder_get_timecode(napi_env env, napi_callback_info info)
{
	napi_status status;

	size_t argc = 1;
	napi_value args[1];
	napi_valuetype type;
	struct ltc_encoder_object *obj;

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
	NAPI_STATUS_RETURN("Error fetching value of argument 1");

	SMPTETimecode timecode;
	ltc_encoder_get_timecode(obj->encoder, &timecode);

	napi_value result;
	status = napi_create_object(env, &result);
	NAPI_STATUS_RETURN("Error creating result object")

	napi_value value;

	NAPI_SETOBJPARAM_NUMBER_ORRETURN("years", timecode.years)
	NAPI_SETOBJPARAM_NUMBER_ORRETURN("months", timecode.months)
	NAPI_SETOBJPARAM_NUMBER_ORRETURN("days", timecode.days)

	NAPI_SETOBJPARAM_NUMBER_ORRETURN("hours", timecode.hours)
	NAPI_SETOBJPARAM_NUMBER_ORRETURN("minutes", timecode.mins)
	NAPI_SETOBJPARAM_NUMBER_ORRETURN("seconds", timecode.secs)

	NAPI_SETOBJPARAM_NUMBER_ORRETURN("frame", timecode.frame)

	NAPI_SETOBJPARAM_STRING_ORRETURN("timezone", timecode.timezone)

	return result;
}

static napi_value encoder_encode_frame(napi_env env, napi_callback_info info)
{
	napi_status status;

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

	struct ltc_encoder_object *obj;
	status = napi_get_value_external(env, args[0], (void **)&obj);
	NAPI_STATUS_RETURN("Error fetching value of argument 1");

	ltc_encoder_buffer_flush(obj->encoder);
	ltc_encoder_encode_frame(obj->encoder);

	return NULL;
}

static napi_value encoder_get_buffer(napi_env env, napi_callback_info info)
{
	napi_status status;

	size_t argc = 1;
	napi_value args[1];
	napi_valuetype type;
	ltcsnd_sample_t *buf;
	int len;

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

	struct ltc_encoder_object *obj;
	status = napi_get_value_external(env, args[0], (void **)&obj);
	NAPI_STATUS_RETURN("Error fetching value of argument 1");

	buf = ltc_encoder_get_bufptr(obj->encoder, &len, 0);
	if (buf == NULL) {
		NAPI_ERROR_RETURN("Error fetching buffer from encoder");
	}

	napi_value buffer;
	status = napi_create_buffer_copy(env, len, buf, NULL, &buffer);
	NAPI_STATUS_RETURN("Error creating buffer");

	return buffer;
}

static napi_value encoder_increment_timecode(napi_env env, napi_callback_info info)
{
	napi_status status;

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

	struct ltc_encoder_object *obj;
	status = napi_get_value_external(env, args[0], (void **)&obj);
	NAPI_STATUS_RETURN("Error fetching value of argument 1");

	ltc_encoder_inc_timecode(obj->encoder);

	return NULL;
}

static napi_value encoder_decrement_timecode(napi_env env, napi_callback_info info)
{
	napi_status status;

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

	struct ltc_encoder_object *obj;
	status = napi_get_value_external(env, args[0], (void **)&obj);
	NAPI_STATUS_RETURN("Error fetching value of argument 1");

	ltc_encoder_dec_timecode(obj->encoder);

	return NULL;
}

static void destroy_ltc_decoder_object(napi_env env, void *data, void *hint)
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

	status = napi_create_external(env, obj, destroy_ltc_decoder_object, NULL, &result);
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
		status = napi_set_named_property(env, result, "drop_frame_format", ltc_df);

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
	napi_value create_ltc_encoder_function;
	napi_value encoder_set_volume_function;
	napi_value encoder_set_filter_function;
	napi_value encoder_set_timecode_function;
	napi_value encoder_get_timecode_function;
	napi_value encoder_encode_frame_function;
	napi_value encoder_get_buffer_function;
	napi_value encoder_increment_timecode_function;
	napi_value encoder_decrement_timecode_function;

	NAPI_CALL(env, napi_create_function(env, "createLTCDecoder", NAPI_AUTO_LENGTH, create_ltc_decoder, NULL, &create_ltc_decoder_function));
	NAPI_CALL(env, napi_set_named_property(env, exports, "createLTCDecoder", create_ltc_decoder_function));

	NAPI_CALL(env, napi_create_function(env, "writeAudio", NAPI_AUTO_LENGTH, write_audio, NULL, &write_audio_function));
	NAPI_CALL(env, napi_set_named_property(env, exports, "writeAudio", write_audio_function));

	NAPI_CALL(env, napi_create_function(env, "readFrame", NAPI_AUTO_LENGTH, read_frame, NULL, &read_frame_function));
	NAPI_CALL(env, napi_set_named_property(env, exports, "readFrame", read_frame_function));

	NAPI_CALL(env, napi_create_function(env, "createLTCEncoder", NAPI_AUTO_LENGTH, create_ltc_encoder, NULL, &create_ltc_encoder_function));
	NAPI_CALL(env, napi_set_named_property(env, exports, "createLTCEncoder", create_ltc_encoder_function));

	NAPI_CALL(env, napi_create_function(env, "encoderSetVolume", NAPI_AUTO_LENGTH, encoder_set_volume, NULL, &encoder_set_volume_function));
	NAPI_CALL(env, napi_set_named_property(env, exports, "encoderSetVolume", encoder_set_volume_function));

	NAPI_CALL(env, napi_create_function(env, "encoderSetFilter", NAPI_AUTO_LENGTH, encoder_set_filter, NULL, &encoder_set_filter_function));
	NAPI_CALL(env, napi_set_named_property(env, exports, "encoderSetFilter", encoder_set_filter_function));

	NAPI_CALL(env, napi_create_function(env, "encoderSetTimecode", NAPI_AUTO_LENGTH, encoder_set_timecode, NULL, &encoder_set_timecode_function));
	NAPI_CALL(env, napi_set_named_property(env, exports, "encoderSetTimecode", encoder_set_timecode_function));

	NAPI_CALL(env, napi_create_function(env, "encoderGetTimecode", NAPI_AUTO_LENGTH, encoder_get_timecode, NULL, &encoder_get_timecode_function));
	NAPI_CALL(env, napi_set_named_property(env, exports, "encoderGetTimecode", encoder_get_timecode_function));

	NAPI_CALL(env, napi_create_function(env, "encoderEncodeFrame", NAPI_AUTO_LENGTH, encoder_encode_frame, NULL, &encoder_encode_frame_function));
	NAPI_CALL(env, napi_set_named_property(env, exports, "encoderEncodeFrame", encoder_encode_frame_function));

	NAPI_CALL(env, napi_create_function(env, "encoderGetBuffer", NAPI_AUTO_LENGTH, encoder_get_buffer, NULL, &encoder_get_buffer_function));
	NAPI_CALL(env, napi_set_named_property(env, exports, "encoderGetBuffer", encoder_get_buffer_function));

	NAPI_CALL(env, napi_create_function(env, "encoderIncrementTimecode", NAPI_AUTO_LENGTH, encoder_increment_timecode, NULL, &encoder_increment_timecode_function));
	NAPI_CALL(env, napi_set_named_property(env, exports, "encoderIncrementTimecode", encoder_increment_timecode_function));

	NAPI_CALL(env, napi_create_function(env, "encoderDecrementTimecode", NAPI_AUTO_LENGTH, encoder_decrement_timecode, NULL, &encoder_decrement_timecode_function));
	NAPI_CALL(env, napi_set_named_property(env, exports, "encoderDecrementTimecode", encoder_decrement_timecode_function));

	return exports;
}

NAPI_MODULE(nodencl, Init)
