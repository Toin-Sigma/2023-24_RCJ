//--------------------------------------------------------------------------------
#include "D_Main.h"
#include "D_I2C.h"
#include "D_SIO.h"
#include "D_EIO.h"
//--------------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------------
void user_sub_1(void);
void user_sub_2(void);
void user_sub_3(void);
void user_main(void);
//--------------------------------------------------------------------------------
// Program Name : 変更値_値の差を直接代入【Main】.C
//--------------------------------------------------------------------------------
void user_sub_1(void)
{
    gV[VAR_A] = gAD[CN1];
    gV[VAR_B] = gAD[CN2];
    gV[VAR_C] = gAD[CN3];
    gV[VAR_D] = gAD[CN4];
    gV[VAR_E] = gAD[CN5];
    gV[VAR_F] = gAD[CN6];
    gV[VAR_G] = gAD[CN7];
    gV[VAR_H] = gAD[CN8];
}
//--------------------------------------------------------------------------------
// Program Name : ボール比べ.C
//--------------------------------------------------------------------------------
void user_sub_2(void)
{
    gV[VAR_U] = 1;  	// U=ボールに一番近いセンサー
    gV[VAR_V] = gV[VAR_A];
    if (gV[VAR_V] <= gV[VAR_B]) {
        gV[VAR_U] = 2;
        gV[VAR_V] = gV[VAR_B];
    } else {
    }
    if (gV[VAR_V] <= gV[VAR_C]) {
        gV[VAR_U] = 3;
        gV[VAR_V] = gV[VAR_C];
    } else {
    }
    if (gV[VAR_V] <= gV[VAR_D]) {
        gV[VAR_U] = 4;
        gV[VAR_V] = gV[VAR_D];
    } else {
    }
    if (gV[VAR_V] <= gV[VAR_E]) {
        gV[VAR_U] = 5;
        gV[VAR_V] = gV[VAR_E];
    } else {
    }
    if (gV[VAR_V] <= gV[VAR_F]) {
        gV[VAR_U] = 6;
        gV[VAR_V] = gV[VAR_F];
    } else {
    }
    if (gV[VAR_V] <= gV[VAR_G]) {
        gV[VAR_U] = 7;
        gV[VAR_V] = gV[VAR_G];
    } else {
    }
    if (gV[VAR_V] <= gV[VAR_H]) {
        gV[VAR_U] = 8;
        gV[VAR_V] = gV[VAR_H];
    } else {
    }
    if (gV[VAR_V] <= 100) {
        gV[VAR_U] = 0;
        gV[VAR_V] = 0;
    } else {
    }
}
//--------------------------------------------------------------------------------
// Program Name : ボール距離.C
//--------------------------------------------------------------------------------
void user_sub_3(void)
{
      	// 左右のセンサー値の合計を算出するプログラム
    if (gV[VAR_U] == 1) {
        gV[VAR_R] = gV[VAR_B];
        gV[VAR_L] = gV[VAR_H];
    } else if (gV[VAR_U] == 2) {
        gV[VAR_R] = gV[VAR_C];
        gV[VAR_L] = gV[VAR_A];
    } else if (gV[VAR_U] == 3) {
        gV[VAR_R] = gV[VAR_D];
        gV[VAR_L] = gV[VAR_B];
    } else if (gV[VAR_U] == 4) {
        gV[VAR_R] = gV[VAR_E];
        gV[VAR_L] = gV[VAR_C];
    } else if (gV[VAR_U] == 5) {
        gV[VAR_R] = gV[VAR_F];
        gV[VAR_L] = gV[VAR_D];
    } else if (gV[VAR_U] == 6) {
        gV[VAR_R] = gV[VAR_G];
        gV[VAR_L] = gV[VAR_E];
    } else if (gV[VAR_U] == 7) {
        gV[VAR_R] = gV[VAR_H];
        gV[VAR_L] = gV[VAR_F];
    } else if (gV[VAR_U] == 8) {
        gV[VAR_R] = gV[VAR_A];
        gV[VAR_L] = gV[VAR_G];
    } else {
        gV[VAR_R] = 0;
        gV[VAR_L] = 0;
    }
    if (gV[VAR_R] + gV[VAR_L] >= 850) {
        gV[VAR_Q] = 1;
        set_Led(0, LED_ON);
    } else if (gV[VAR_R] == 0 && gV[VAR_L] == 0) {
        gV[VAR_Q] = 0;
        set_Led(0, LED_OFF);
    } else {
        gV[VAR_Q] = 2;
        set_Led(0, LED_OFF);
    }
}
//--------------------------------------------------------------------------------
// Program Name : 研究.C
//--------------------------------------------------------------------------------
void user_main(void)
{
    gV[VAR_X] = -70;
    gV[VAR_Y] = -30;
    gV[VAR_M] = get_bno(0);
    gV[VAR_P] = 0;
    gV[VAR_O] = 0;
    gV[VAR_Z] = 0;
    while (TRUE) {
        int attackp;  	// 自陣ゴールに戻る時の制御
        int gkok;  	// キーパーとしてボールの前後にあるかどうかのフラグ変数（前後にないならライン制御割り込みでボールの制御に）
        int fline;  	// 前ライン踏むか踏まないかフラグ変数
        int rline;  	// 同様に右ライン
        int bline;  	// 同様に後ろライン
        int lline;  	// 同様に左ライン
        int linenow;  	// どのセンサーが白線を感知しているか、16パターン
        int justbefore;  	// 直近のラインを踏んだセンサ
        int predm;  	// たいちゃんPD
        int difop;  	// 〃
        int gkreturn;
        int online;
        if (sub_io_get_gV(1, VAR_Z) == 1) {
            clr_timer(0);
            clr_timer(1);
            while (TRUE) {
                gV[VAR_N] = get_bno(0);
                gV[VAR_O] = gV[VAR_N] - gV[VAR_M];
                if (gV[VAR_O] >= 180) {
                    gV[VAR_O] = gV[VAR_O] - 360;
                } else if (gV[VAR_O] <= -180) {
                    gV[VAR_O] = gV[VAR_O] + 360;
                } else {
                }
                if ((90 <= gV[VAR_O]) && (gV[VAR_O] <= 180)) {
                    gV[VAR_P] = gV[VAR_O] * 0.7;
                } else if ((45 <= gV[VAR_O]) && (gV[VAR_O] <= 90)) {
                    gV[VAR_P] = 55;
                } else if ((25 <= gV[VAR_O]) && (gV[VAR_O] <= 45)) {
                    gV[VAR_P] = gV[VAR_O] * 1.15;
                } else if ((10 <= gV[VAR_O]) && (gV[VAR_O] <= 25)) {
                    gV[VAR_P] = 23;
                } else {
                }
                if ((-180 <= gV[VAR_O]) && (gV[VAR_O] <= -90)) {
                    gV[VAR_P] = gV[VAR_O] * 0.7;
                } else if ((-90 <= gV[VAR_O]) && (gV[VAR_O] <= -45)) {
                    gV[VAR_P] = -55;
                } else if ((-45 <= gV[VAR_O]) && (gV[VAR_O] <= -25)) {
                    gV[VAR_P] = gV[VAR_O] * 1.15;
                } else if ((-25 <= gV[VAR_O]) && (gV[VAR_O] <= -10)) {
                    gV[VAR_P] = -23;
                }
                difop = (predm - gV[VAR_P]) * 0.1;
                gV[VAR_P] = gV[VAR_P] - difop;
                if ((gV[VAR_O] >= 17) || (gV[VAR_O] <= -17)) {
                    gPwm[0] = gV[VAR_P] < 0 ? (gV[VAR_P] * -1) | 0x80 : gV[VAR_P];
                    gPwm[1] = gV[VAR_P] < 0 ? gV[VAR_P] * -1 : gV[VAR_P] | 0x80;
                    gPwm[2] = gV[VAR_P] < 0 ? (gV[VAR_P] * -1) | 0x80 : gV[VAR_P];
                    gPwm[3] = gV[VAR_P] < 0 ? gV[VAR_P] * -1 : gV[VAR_P] | 0x80;
                    pwm_out();
                } else {
                }
                predm = gV[VAR_P];
                if (sub_io_get_gV(1, VAR_F) >= 50) {
                    fline = 1;
                } else {
                    fline = 0;
                }
                if (sub_io_get_gV(1, VAR_R) >= 50) {
                    rline = 1;
                } else {
                    rline = 0;
                }
                if (sub_io_get_gV(1, VAR_B) >= 50) {
                    bline = 1;
                } else {
                    bline = 0;
                }
                if (sub_io_get_gV(1, VAR_L) >= 50) {
                    lline = 1;
                } else {
                    lline = 0;
                }
                if ((fline == 1) && (rline == 0) && (bline == 0) && (lline == 0)) {
                    online = 0;
                    linenow = 1;
                    justbefore = 1;
                } else if ((fline == 0) && (rline == 1) && (bline == 0) && (lline == 0)) {
                    online = 0;
                    linenow = 2;
                    justbefore = 2;
                } else if ((fline == 1) && (rline == 1) && (bline == 0) && (lline == 0)) {
                    online = 0;
                    linenow = 12;
                    justbefore = 12;
                } else if ((fline == 0) && (rline == 0) && (bline == 1) && (lline == 0)) {
                    online = 0;
                    linenow = 3;
                    justbefore = 3;
                } else if ((fline == 1) && (rline == 0) && (bline == 1) && (lline == 0)) {
                    online = 0;
                    linenow = 13;
                    justbefore = 13;
                } else if ((fline == 0) && (rline == 1) && (bline == 1) && (lline == 0)) {
                    online = 0;
                    linenow = 23;
                    justbefore = 23;
                } else if ((fline == 1) && (rline == 1) && (bline == 1) && (lline == 0)) {
                    online = 0;
                    linenow = 123;
                    justbefore = 123;
                } else if ((fline == 0) && (rline == 0) && (bline == 0) && (lline == 1)) {
                    online = 0;
                    linenow = 4;
                    justbefore = 4;
                } else if ((fline == 1) && (rline == 0) && (bline == 0) && (lline == 1)) {
                    online = 0;
                    linenow = 14;
                    justbefore = 14;
                } else if ((fline == 0) && (rline == 1) && (bline == 0) && (lline == 1)) {
                    online = 1;
                    linenow = 24;
                    justbefore = 24;
                } else if ((fline == 1) && (rline == 1) && (bline == 0) && (lline == 1)) {
                    online = 1;
                    linenow = 124;
                    justbefore = 124;
                } else if ((fline == 0) && (rline == 0) && (bline == 1) && (lline == 1)) {
                    online = 0;
                    linenow = 34;
                    justbefore = 34;
                } else if ((fline == 1) && (rline == 0) && (bline == 1) && (lline == 1)) {
                    online = 0;
                    linenow = 134;
                    justbefore = 134;
                } else if ((fline == 0) && (rline == 1) && (bline == 1) && (lline == 1)) {
                    online = 1;
                    linenow = 234;
                    justbefore = 234;
                } else if ((fline == 1) && (rline == 1) && (bline == 1) && (lline == 1)) {
                    online = 0;
                    linenow = 1234;
                    justbefore = 1234;
                } else {
                    online = 0;
                    linenow = 0;
                }
                user_sub_1();
                user_sub_2();
                user_sub_3();
                if (linenow == 24) {
                    gPwm[0] = 0x00;
                    gPwm[1] = 0x00;
                    gPwm[2] = 0x00;
                    gPwm[3] = 0x00;
                    pwm_out();
                } else {
                    if (rline == 0) {
                        gPwm[0] = gV[VAR_X] < 0 ? (gV[VAR_X] * -1) | 0x80 : gV[VAR_X];
                        gPwm[1] = gV[VAR_X] < 0 ? gV[VAR_X] * -1 : gV[VAR_X] | 0x80;
                        gPwm[2] = gV[VAR_X] < 0 ? gV[VAR_X] * -1 : gV[VAR_X] | 0x80;
                        gPwm[3] = gV[VAR_X] < 0 ? (gV[VAR_X] * -1) | 0x80 : gV[VAR_X];
                        pwm_out();
                    } else if (lline == 0) {
                        gPwm[0] = gV[VAR_X] < 0 ? gV[VAR_X] * -1 : gV[VAR_X] | 0x80;
                        gPwm[1] = gV[VAR_X] < 0 ? (gV[VAR_X] * -1) | 0x80 : gV[VAR_X];
                        gPwm[2] = gV[VAR_X] < 0 ? (gV[VAR_X] * -1) | 0x80 : gV[VAR_X];
                        gPwm[3] = gV[VAR_X] < 0 ? gV[VAR_X] * -1 : gV[VAR_X] | 0x80;
                        pwm_out();
                    } else if (fline == 1) {
                        if (gkok == 1) {
                            gPwm[0] = gV[VAR_Y] < 0 ? (gV[VAR_Y] * -1) | 0x80 : gV[VAR_Y];
                            gPwm[1] = gV[VAR_Y] < 0 ? (gV[VAR_Y] * -1) | 0x80 : gV[VAR_Y];
                            gPwm[2] = gV[VAR_Y] < 0 ? (gV[VAR_Y] * -1) | 0x80 : gV[VAR_Y];
                            gPwm[3] = gV[VAR_Y] < 0 ? (gV[VAR_Y] * -1) | 0x80 : gV[VAR_Y];
                            pwm_out();
                        } else {
                        }
                    } else if (bline == 1) {
                        if (gkok == 1) {
                            gPwm[0] = gV[VAR_Y] < 0 ? gV[VAR_Y] * -1 : gV[VAR_Y] | 0x80;
                            gPwm[1] = gV[VAR_Y] < 0 ? gV[VAR_Y] * -1 : gV[VAR_Y] | 0x80;
                            gPwm[2] = gV[VAR_Y] < 0 ? gV[VAR_Y] * -1 : gV[VAR_Y] | 0x80;
                            gPwm[3] = gV[VAR_Y] < 0 ? gV[VAR_Y] * -1 : gV[VAR_Y] | 0x80;
                            pwm_out();
                        } else {
                        }
                    } else {
                    }
                }
                if (gV[VAR_U] == 1) {
                    gkok = 1;
                    gPwm[0] = 0x00;
                    gPwm[1] = 0x00;
                    gPwm[2] = 0x00;
                    gPwm[3] = 0x00;
                    pwm_out();
                    continue;
                } else if (gV[VAR_U] == 2) {
                    gkok = 0;
                    gPwm[0] = gV[VAR_X] < 0 ? gV[VAR_X] * -1 : gV[VAR_X] | 0x80;
                    gPwm[1] = gV[VAR_X] < 0 ? (gV[VAR_X] * -1) | 0x80 : gV[VAR_X];
                    gPwm[2] = gV[VAR_X] < 0 ? (gV[VAR_X] * -1) | 0x80 : gV[VAR_X];
                    gPwm[3] = gV[VAR_X] < 0 ? gV[VAR_X] * -1 : gV[VAR_X] | 0x80;
                    pwm_out();
                    continue;
                } else if (gV[VAR_U] == 3) {
                    gkok = 0;
                    gPwm[0] = gV[VAR_X] < 0 ? gV[VAR_X] * -1 : gV[VAR_X] | 0x80;
                    gPwm[1] = gV[VAR_X] < 0 ? (gV[VAR_X] * -1) | 0x80 : gV[VAR_X];
                    gPwm[2] = gV[VAR_X] < 0 ? (gV[VAR_X] * -1) | 0x80 : gV[VAR_X];
                    gPwm[3] = gV[VAR_X] < 0 ? gV[VAR_X] * -1 : gV[VAR_X] | 0x80;
                    pwm_out();
                    continue;
                } else if (gV[VAR_U] == 4) {
                    gkok = 0;
                    gPwm[0] = gV[VAR_X] < 0 ? gV[VAR_X] * -1 : gV[VAR_X] | 0x80;
                    gPwm[1] = gV[VAR_X] < 0 ? (gV[VAR_X] * -1) | 0x80 : gV[VAR_X];
                    gPwm[2] = gV[VAR_X] < 0 ? (gV[VAR_X] * -1) | 0x80 : gV[VAR_X];
                    gPwm[3] = gV[VAR_X] < 0 ? gV[VAR_X] * -1 : gV[VAR_X] | 0x80;
                    pwm_out();
                    continue;
                } else if (gV[VAR_U] == 5) {
                    gkok = 1;
                    gPwm[0] = 0x00;
                    gPwm[1] = 0x00;
                    gPwm[2] = 0x00;
                    gPwm[3] = 0x00;
                    pwm_out();
                    continue;
                } else if (gV[VAR_U] == 6) {
                    gkok = 0;
                    gPwm[0] = gV[VAR_X] < 0 ? gV[VAR_X] * -1 : gV[VAR_X] | 0x80;
                    gPwm[1] = gV[VAR_X] < 0 ? (gV[VAR_X] * -1) | 0x80 : gV[VAR_X];
                    gPwm[2] = gV[VAR_X] < 0 ? (gV[VAR_X] * -1) | 0x80 : gV[VAR_X];
                    gPwm[3] = gV[VAR_X] < 0 ? gV[VAR_X] * -1 : gV[VAR_X] | 0x80;
                    pwm_out();
                    continue;
                } else if (gV[VAR_U] == 7) {
                    gkok = 0;
                    gPwm[0] = gV[VAR_X] < 0 ? gV[VAR_X] * -1 : gV[VAR_X] | 0x80;
                    gPwm[1] = gV[VAR_X] < 0 ? (gV[VAR_X] * -1) | 0x80 : gV[VAR_X];
                    gPwm[2] = gV[VAR_X] < 0 ? (gV[VAR_X] * -1) | 0x80 : gV[VAR_X];
                    gPwm[3] = gV[VAR_X] < 0 ? gV[VAR_X] * -1 : gV[VAR_X] | 0x80;
                    pwm_out();
                    continue;
                } else if (gV[VAR_U] == 8) {
                    gkok = 0;
                    gPwm[0] = gV[VAR_X] < 0 ? gV[VAR_X] * -1 : gV[VAR_X] | 0x80;
                    gPwm[1] = gV[VAR_X] < 0 ? (gV[VAR_X] * -1) | 0x80 : gV[VAR_X];
                    gPwm[2] = gV[VAR_X] < 0 ? (gV[VAR_X] * -1) | 0x80 : gV[VAR_X];
                    gPwm[3] = gV[VAR_X] < 0 ? gV[VAR_X] * -1 : gV[VAR_X] | 0x80;
                    pwm_out();
                    continue;
                } else {
                    gkok = 1;
                    continue;
                }
            }
        } else {
        }
    }
}
//--------------------------------------------------------------------------------
