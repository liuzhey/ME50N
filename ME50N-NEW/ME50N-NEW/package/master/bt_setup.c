#include "tools.h"

void BTNamePINSetup(int n)
{
    int ret, len;;
    char buffer[32], tip[64];
    NDK_ScrClrs();
    DrawTitles(0, 0, lcd_w, font_h + 4, bt_lists[glanguage][n], 1);
    if (n) {
        if (NDK_BTGetPIN(buffer) != NDK_OK) goto err;
    }
    else {
        if (NDK_BTGetLocalName(buffer) != NDK_OK) goto err;
    }
    sprintf(tip, "%s:%s", bt_prompt[n][glanguage], buffer);
    NDK_ScrDispString(0, font_h + 6, tip, 0);
    NDK_ScrDispString(0, font_h * 2 + 2, bt_prompt[2][glanguage], 0);
    if (n) {
        ret = NDK_KbGetInput(buffer, 6, 6, (uint*)&len, INPUTDISP_NORMAL,
                    0, INPUT_CONTRL_NOLIMIT_ERETURN);
    }
    else {
        ret = NDK_KbGetInput(buffer, 2, 15, (uint*)&len, INPUTDISP_NORMAL,
            0, INPUT_CONTRL_NOLIMIT_ERETURN);
    }
    if (ret < 0) return;
    if (n) {
        ret = NDK_BTSetPIN(buffer);
    }
    else {
        ret = NDK_BTSetLocalName(buffer);
    }
    if (ret != NDK_OK) {
        NDK_ScrDispString(0, font_h * 3, all_prompt[1][glanguage], 0);
    }
    else NDK_ScrDispString(0, font_h * 3, all_prompt[0][glanguage], 0);
    NDK_KbGetCode(5, NULL);
    return;
err:
    NDK_ScrDispString(0, font_h + 6, bt_prompt[3][glanguage], 0);
    NDK_KbGetCode(5, NULL);
    return;
}

void BTChannelSetup(void)
{
    char ch, tip[64];
    int len, ret;

    NDK_ScrClrs();
    DrawTitles(0, 0, lcd_w, font_h + 4, bt_lists[glanguage][2], 1);
    if (NDK_BTGetChannel(&ch) != NDK_OK) goto err;
    sprintf(tip, "%s:%d", bt_prompt[4][glanguage], ch);
    NDK_ScrDispString(0, font_h + 6, tip, 0);
    NDK_ScrDispString(0, font_h * 2 + 2, bt_prompt[2][glanguage], 0);

    ret = NDK_KbGetInput(tip, 1, 2, (uint*)&len, INPUTDISP_NORMAL,
                0, INPUT_CONTRL_NOLIMIT_ERETURN);
    if (ret < 0) return;
    ch = atoi(tip);
    ret = NDK_BTSetChannel(ch);
    if (ret != NDK_OK) {
        NDK_ScrDispString(0, font_h * 3, all_prompt[1][glanguage], 0);
    }
    else NDK_ScrDispString(0, font_h * 3, all_prompt[0][glanguage], 0);
    NDK_KbGetCode(5, NULL);
    return;
err:
    NDK_ScrDispString(0, font_h + 6, bt_prompt[3][glanguage], 0);
    NDK_KbGetCode(5, NULL);
    return;
}

void BTPINCodeSetup(void)
{
    int key;
    NDK_ScrClrs();
    DrawTitles(0, 0, lcd_w, font_h + 2, bt_lists[glanguage][3], 1);
    NDK_ScrDispString(0, font_h + 4, "1.JUST WORK", 0);
    NDK_ScrDispString(0, font_h * 2 + 4, "2.PIN CODE ", 0);
    NDK_ScrDispString(0, font_h * 3 + 4, "3.SSP PIN", 0);
    NDK_ScrDispString(0, font_h * 4 + 4, "4.PASSKEY", 0);
rep:
    NDK_KbGetCode(0, &key);
    if (key == K_ESC) return;
    if (key == '1' || key == '2' || key == '3' || key == '4') {
        NDK_ScrClrLine(font_h + 4, lcd_h);
        NDK_ScrDispString(0, font_h + 6, "Set ... ", 0);
        if (NDK_BTSetPairingMode((char )key - '1') != NDK_OK) {
            NDK_ScrDispString(48, font_h + 4, "Failed", 0);
        }
        else NDK_ScrDispString(48, font_h + 4, "OK", 0);
        NDK_KbGetCode(0, NULL);
        return;
    }
    else goto rep;
}

void BTParamSetup(void)
{
    int sel;
    NDK_PortOpen(PORT_NUM_BT, NULL);
    NDK_ScrClrs();
    DrawTitles(0, 24, 128, 16, "Enter CMD Mode", 1);
    NDK_BTEnterCommand();
    while (1) {
        sel = DrawList(bt_titles[glanguage],
                (char **)bt_lists[glanguage], 4, 0);
        if (sel < 0) break;
        switch (sel) {
        case 0: BTNamePINSetup(0); break;
        case 1: BTNamePINSetup(1); break;
        case 2: BTChannelSetup(); break;
        case 3: BTPINCodeSetup(); break;
        default: break;
        }
    }
    NDK_PortClose(PORT_NUM_BT);
}
