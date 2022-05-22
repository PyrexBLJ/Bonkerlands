#pragma once

#define OSK_LIMIT_LENGTH	(100)

typedef enum osk_callback_type {
	osk_wide,
	osk_ansi,
	osk_closed
} osk_callback_type_t;

typedef union message_buff
{
	wchar_t wide[100];
	char ansi[100];
};

typedef struct osk_callback_data {
	osk_callback_type_t type;
	message_buff message;
	int len;
} osk_callback_data_t;

typedef void (*osk_callback_t)(osk_callback_data_t* data, void* arg);

class COsk
{
public:
	COsk();
	~COsk();
	bool initClass(void);
	bool termClass(void);
	bool start(osk_callback_type_t type, osk_callback_t func, void* arg);
	bool abortOsk(void);

private:
	static const int sSlot = 2; /* 0, 1: in use or reserved */
	wchar_t mpTextBuffer[OSK_LIMIT_LENGTH + 1];
	osk_callback_t mOskCallbackFunc;
	void* mOskCallbackArg;
	osk_callback_type mType;
	uint32_t mFlags;

	static void sysutilCb(uint64_t status, uint64_t param, void* userdata);
	void sysutilCbInternal(uint64_t status);
	void stopOsk(void);
	void closeOsk(void);
};

static COsk key;


