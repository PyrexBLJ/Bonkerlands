/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 475.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sys/memory.h>
#include <sysutil/sysutil_sysparam.h>
#include <sysutil/sysutil_oskdialog.h>

/* game */
#include "include.h"


#define OSK_MESSAGE	(L"chat message")
#define OSK_INIT_TEXT	(L"")

/* mFlags */
#define OSK_IN_USE	(0x00000001)

COsk::COsk()
{
}

COsk::~COsk()
{
}

bool COsk::initClass(void)
{
	memset(mpTextBuffer, 0, sizeof(*mpTextBuffer));
	mFlags = 0;
	return (true);
}

bool COsk::termClass(void)
{
	return (true);
}

bool COsk::start(osk_callback_type_t type, osk_callback_t func, void* arg)
{
	CellOskDialogInputFieldInfo info;
	CellOskDialogParam param;
	CellOskDialogPoint pos;
	int ret;
	mType = type;
	if (mFlags & OSK_IN_USE) {
		return (true);
	}
	if (func == NULL) {
		printf("%s, invalid func\n", __FUNCTION__);
		goto fail10;
	}
	ret = cellSysutilRegisterCallback(sSlot, sysutilCb, this);
	if (ret != CELL_OK) {
		printf("cellSysutilRegisterCallback() = 0x%x\n", ret);
		goto fail20;
	}
	info.message = (uint16_t*)OSK_MESSAGE;
	info.init_text = (uint16_t*)OSK_INIT_TEXT;
	info.limit_length = OSK_LIMIT_LENGTH;
	pos.x = 0.0f;
	pos.y = 0.0f;
	param.allowOskPanelFlg = CELL_OSKDIALOG_PANELMODE_DEFAULT;
	param.firstViewPanel = CELL_OSKDIALOG_PANELMODE_DEFAULT;
	param.controlPoint = pos;
	param.prohibitFlgs = CELL_OSKDIALOG_NO_RETURN;
	ret = cellOskDialogAddSupportLanguage(CELL_OSKDIALOG_PANELMODE_KOREAN);
	if (ret < 0) {
		printf("cellOskDialogAddSupportLanguage() failed. ret = 0x%x\n", ret);
		goto fail30;
	}
	ret = cellOskDialogSetKeyLayoutOption(CELL_OSKDIALOG_10KEY_PANEL |
		CELL_OSKDIALOG_FULLKEY_PANEL);
	if (ret < 0) {
		printf("cellOskDialogSetKeyLayoutOption() failed. ret = 0x%x\n", ret);
		goto fail30;
	}
	ret = cellOskDialogLoadAsync(SYS_MEMORY_CONTAINER_ID_INVALID,
		&param, &info);
	if (ret < 0) {
		printf("cellOskDialogLoadAsync() failed. ret = 0x%x \n", ret);
		goto fail30;
	}
	mOskCallbackFunc = func;
	mOskCallbackArg = arg;
	mFlags |= OSK_IN_USE;
	return (true);

fail30:
	cellSysutilUnregisterCallback(sSlot);
fail20:
fail10:
	return (false);
}

bool COsk::abortOsk(void)
{
	int ret;

	if ((mFlags & OSK_IN_USE) == 0) {
		return (false);
	}
	ret = cellOskDialogAbort();
	if (ret < 0) {
		printf("cellOskDialogAbort() failed. ret = 0x%x \n", ret);
	}
	return (true);
}

void COsk::stopOsk(void)
{
	CellOskDialogCallbackReturnParam param;
	osk_callback_data_t data;
	int ret;

	param.result = CELL_OSKDIALOG_INPUT_FIELD_RESULT_OK;
	param.numCharsResultString = OSK_LIMIT_LENGTH;
	param.pResultString = (uint16_t*)mpTextBuffer;
	ret = cellOskDialogUnloadAsync(&param);
	if (ret < 0) {
		printf("cellOskDialogUnloadAsync() failed. ret = 0x%x \n", ret);
	}
	if (param.result == CELL_OSKDIALOG_INPUT_FIELD_RESULT_OK) {
		if (wcslen(mpTextBuffer) > 0) {
			memset(&data, 0, sizeof(data));
			auto widechar_to_multibyte = [](const wchar_t* value, size_t size, char* buffer) {
				for (int i = 0; i < size; i++)
					buffer[i] = (char)value[i];
			};
			char buf[256];

			if (mType == osk_ansi)
			{
				widechar_to_multibyte(mpTextBuffer, wcslen(mpTextBuffer) * 2, buf);
				strcpy(data.message.ansi, buf);
				data.len = strlen(buf);
			}
			else
			{
				wcscpy(data.message.wide, mpTextBuffer);
				data.len = wcslen(mpTextBuffer) * 2;
			}
			data.type = mType;


			(*mOskCallbackFunc)(&data, mOskCallbackArg);

			libpsutil::sleep(500);
		}
	}
	mFlags &= ~OSK_IN_USE;
}

void COsk::closeOsk(void)
{
	osk_callback_data_t data;
	int ret;

	ret = cellSysutilUnregisterCallback(sSlot);
	if (ret < 0) {
		printf("cellSysutilUnregisterCallback() failed. ret = 0x%x\n", ret);
	}
	memset(&data, 0, sizeof(data));
	data.type = osk_closed;
	//(*mOskCallbackFunc)(&data, mOskCallbackArg);
}

void COsk::sysutilCbInternal(uint64_t status)
{
	switch (status) {
	case CELL_SYSUTIL_OSKDIALOG_LOADED:
		break;
	case CELL_SYSUTIL_OSKDIALOG_FINISHED:
		stopOsk();
		break;
	case CELL_SYSUTIL_OSKDIALOG_UNLOADED:
		closeOsk();
		break;
	default:
		break;
	}
}

void COsk::sysutilCb(uint64_t status, uint64_t param, void* userdata)
{
	COsk* self = static_cast<COsk*>(userdata);

	(void)param;
	self->sysutilCbInternal(status);
}

