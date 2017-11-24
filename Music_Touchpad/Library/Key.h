/*
 * Author: Do Tieu Thien
 * Date: 21/11/2017
 */

#ifndef KEY_H_
#define KEY_H_
int base_cap[8];
struct Element
{
    unsigned char   *Pxsel;       // Pointer to Pxsel register
    unsigned char   *Pxsel2;      // Pointer to Pxsel2 register
    unsigned char   *Pxdir;       // Pointer to Pxdir register
    unsigned char   *Pxout;       // Pointer to Pxout register
    unsigned char   inputBits;    // Input bit
    int             *baseline;    // Pointer to baseline capacitance
};


//**********************************************************************************************************
// Define 8 Key for Touch-pad
//**********************************************************************************************************
const struct Element KEY_1 =
{
    .Pxsel = (unsigned char *)&P2SEL,
    .Pxsel2 = (unsigned char *)&P2SEL2,
    .Pxdir = (unsigned char *)&P2DIR,
    .inputBits = BIT0,
    .baseline = &base_cap[0]
};

const struct Element KEY_2 =
{
    .Pxsel = (unsigned char *)&P2SEL,
    .Pxsel2 = (unsigned char *)&P2SEL2,
    .Pxdir = (unsigned char *)&P2DIR,
    .inputBits = BIT1,
    .baseline = &base_cap[1]
};

const struct Element KEY_3 =
{
    .Pxsel = (unsigned char *)&P2SEL,
    .Pxsel2 = (unsigned char *)&P2SEL2,
    .Pxdir = (unsigned char *)&P2DIR,
    .inputBits = BIT2,
    .baseline = &base_cap[2]
};

const struct Element KEY_4 =
{
    .Pxsel = (unsigned char *)&P2SEL,
    .Pxsel2 = (unsigned char *)&P2SEL2,
    .Pxdir = (unsigned char *)&P2DIR,
    .inputBits = BIT3,
    .baseline = &base_cap[3]
};

const struct Element KEY_5 =
{
    .Pxsel = (unsigned char *)&P2SEL,
    .Pxsel2 = (unsigned char *)&P2SEL2,
    .Pxdir = (unsigned char *)&P2DIR,
    .inputBits = BIT4,
    .baseline = &base_cap[4]
};

const struct Element KEY_6 =
{
    .Pxsel = (unsigned char *)&P2SEL,
    .Pxsel2 = (unsigned char *)&P2SEL2,
    .Pxdir = (unsigned char *)&P2DIR,
    .inputBits = BIT5,
    .baseline = &base_cap[5]
};

const struct Element KEY_7 =
{
    .Pxsel = (unsigned char *)&P2SEL,
    .Pxsel2 = (unsigned char *)&P2SEL2,
    .Pxdir = (unsigned char *)&P2DIR,
    .inputBits = BIT7,
    .baseline = &base_cap[6]
};

const struct Element KEY_8 =
{
    .Pxsel = (unsigned char *)&P2SEL,
    .Pxsel2 = (unsigned char *)&P2SEL2,
    .Pxdir = (unsigned char *)&P2DIR,
    .inputBits = BIT6,
    .baseline = &base_cap[7]
};

const struct Element *KEY[] = {&KEY_1, &KEY_2, &KEY_3, &KEY_4, &KEY_5, &KEY_6, &KEY_7, &KEY_8};

#endif /* KEY_H_ */
