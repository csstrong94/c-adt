#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include "atom.h"
#include "assert.h"

#define __MAX_DIGITS__      43
#define TABLE_SIZE          2048
#define SCATTER_LEN         256
#define NELEMS(x)           ((sizeof(x))/(sizeof(x[0])))

/* 
 * atom consisting of atom length, data, and a pointer 
 * to an atom in order to handle hash collisions.
 * if there is a collision, the atom is added to a 
 * linked list at the hashed location in the array
 * 
 * struct size: 24 bytes with x86-64 arch
 */
static struct atom {
    struct atom* link; //* 8 bytes
    int len;           //* 4 bytes, 4 bytes padding
    char data[1];        //* 8 bytes
    

} *buckets[2048];


static unsigned long scatter[] = {
    1804289383, 846930886, 1681692777, 1714636915, 1957747793, 424238335, 719885386, 1649760492, 
    596516649, 1189641421, 1025202362, 1350490027, 783368690, 1102520059, 2044897763, 1967513926, 
    1365180540, 1540383426, 304089172, 1303455736, 35005211, 521595368, 294702567, 1726956429, 
    336465782, 861021530, 278722862, 233665123, 2145174067, 468703135, 1101513929, 1801979802, 
    1315634022, 635723058, 1369133069, 1125898167, 1059961393, 2089018456, 628175011, 1656478042, 
    1131176229, 1653377373, 859484421, 1914544919, 608413784, 756898537, 1734575198, 1973594324, 
    149798315, 2038664370, 1129566413, 184803526, 412776091, 1424268980, 1911759956, 749241873, 
    137806862, 42999170, 982906996, 135497281, 511702305, 2084420925, 1937477084, 1827336327, 
    572660336, 1159126505, 805750846, 1632621729, 1100661313, 1433925857, 1141616124, 84353895, 
    939819582, 2001100545, 1998898814, 1548233367, 610515434, 1585990364, 1374344043, 760313750, 
    1477171087, 356426808, 945117276, 1889947178, 1780695788, 709393584, 491705403, 1918502651, 
    752392754, 1474612399, 2053999932, 1264095060, 1411549676, 1843993368, 943947739, 1984210012, 
    855636226, 1749698586, 1469348094, 1956297539, 1036140795, 463480570, 2040651434, 1975960378, 
    317097467, 1892066601, 1376710097, 927612902, 1330573317, 603570492, 1687926652, 660260756, 
    959997301, 485560280, 402724286, 593209441, 1194953865, 894429689, 364228444, 1947346619, 
    221558440, 270744729, 1063958031, 1633108117, 2114738097, 2007905771, 1469834481, 822890675, 
    1610120709, 791698927, 631704567, 498777856, 1255179497, 524872353, 327254586, 1572276965, 
    269455306, 1703964683, 352406219, 1600028624, 160051528, 2040332871, 112805732, 1120048829, 
    378409503, 515530019, 1713258270, 1573363368, 1409959708, 2077486715, 1373226340, 1631518149, 
    200747796, 289700723, 1117142618, 168002245, 150122846, 439493451, 990892921, 1760243555, 
    1231192379, 1622597488, 111537764, 338888228, 2147469841, 438792350, 1911165193, 269441500, 
    2142757034, 116087764, 1869470124, 155324914, 8936987, 1982275856, 1275373743, 387346491, 
    350322227, 841148365, 1960709859, 1760281936, 771151432, 1186452551, 1244316437, 971899228, 
    1476153275, 213975407, 1139901474, 1626276121, 653468858, 2130794395, 1239036029, 1884661237, 
    1605908235, 1350573793, 76065818, 1605894428, 1789366143, 1987231011, 1875335928, 1784639529,
     2103318776, 1597322404, 1939964443, 2112255763, 1432114613, 1067854538, 352118606, 1782436840, 
     1909002904, 165344818, 1395235128, 532670688, 1351797369, 492067917, 1504569917, 680466996, 
     706043324, 496987743, 159259470, 1359512183, 480298490, 1398295499, 1096689772, 2086206725, 
     601385644, 1172755590, 1544617505, 243268139, 1012502954, 1272469786, 2027907669, 968338082, 
     722308542, 1820388464, 933110197, 6939507, 740759355, 1285228804, 1789376348, 502278611, 
     1450573622, 1037127828, 1034949299, 654887343, 1529195746, 392035568, 1335354340, 87755422, 
     889023311, 1494613810, 1447267605, 1369321801, 745425661, 396473730, 1308044878, 1346811305
};


const char* Atom_string(const char* str) {
    assert(str);
    int i = strlen(str);
    return Atom_new(str, i);
}

const char* Atom_int(long n) {
    char buf[__MAX_DIGITS__];
    char* s = buf + sizeof(buf);
    unsigned long m;
    

    /**
     * take absolute value of n. if it is long_min,
     * take long max and add one
     */

    if (n == LONG_MIN) {
        m = LONG_MAX + 1L;
    }

    else if (n < 0) {
        m = -n;
    }

    else {
        m = n;
    }

    do {
        *--s = (m % 10) + '0';
    }
    while ((m /= 10) > 0);
    if (n < 0) {
        *--s = '-';
    }

    return Atom_new(s, (buf + sizeof(buf) - s ));
}

/**
 * make a new atom and add it to hash table.
 * since we are making a chaining hash table,
 * any collisions will be added to the array index
 * in a linked list
 * 
 * 1. compute our hash (at first it will be a constant for test)
 * 2. check if data is already in a bucket
 * 3. if so, add item to linked list
 * 3a. if not, add to bucket
 * 4. return the string added
 */

const char* Atom_new(const char* str, uint32_t len) {

    unsigned long hash;
    struct atom* p;
    int i;
    
    assert(str);
    assert(len >= 0);

    for (hash = 0, i = 0; i < len; i++) {
        hash = (hash << 1) + scatter[(unsigned char)str[i]];
    }

    hash %= TABLE_SIZE; // or you can use NELEMS(buckets)

    for (p = buckets[hash]; p; p = p->link) {
        if (p->len == len) {
            for (i = 0; i < len && str[i] == p->data[i]; ) {
                i++;
            }
        }
        if (i == len) {
            return p->data;
        }

    }

    p = malloc(sizeof(*p) + len);
    p->len = len;
   

    if (len > 0) {
        memcpy(p->data, str, len);
    }

    p->data[len] = '\0';

    p->link = buckets[hash];

    buckets[hash] = p;

    return p->data;

}



/**
 * first we create an atom to be used as a pointer
 * we need to interate through the array, and then iterate
 * through the linked list chain of a populated bucket
 */


int Atom_length(const char* str) {
    struct atom* p;
    int i, len;
    unsigned long hash;

    len = strlen(str);
    for (hash = 0, i = 0; i < len; i++) {
        hash = (hash << 1) + scatter[(unsigned char)str[i]];
    }
    hash %= NELEMS(buckets);
    
    assert(str);
    if (buckets[hash]) {
        for (p = buckets[hash]; p; p = p->link ) {
            if (p->data == str) {
                return p->len;
            }
        }
    }
    
    assert(0);
    return 0;
}



