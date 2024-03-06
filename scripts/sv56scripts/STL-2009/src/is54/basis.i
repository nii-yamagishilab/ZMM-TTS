/**************************************************************************

                (C) Copyright 1990, Motorola Inc., U.S.A.

Note:  Reproduction and use for the development of North American digital
       cellular standards or development of digital speech coding
       standards within the International Telecommunications Union -
       Telecommunications Standardization Sector is authorized by Motorola 
       Inc.  No other use is intended or authorized.

       The availability of this material does not provide any license
       by implication, estoppel, or otherwise under any patent rights
       of Motorola Inc. or others covering any use of the contents
       herein.

       Any copies or derivative works must incude this and all other
       proprietary notices.


Systems Research Laboratories
Chicago Corporate Research and Development Center
Motorola Inc.

**************************************************************************/
/*-----------------------------------------------------------------*/
/**/
/*	basisVecsAndGsp0.i -- basis vectors and gsp0 tables.*/
/**/
/*-----------------------------------------------------------------*/
/**/
/*	Written by: Matt Hartman*/
/**/
/*-------------------------------------------------------------*/
tmpPtr = BASIS - 1;
 /* THE CONSTANT DIVIDING THE BASIS VECTORS ->   256*/
 /*   */
 /* BASIS VECTOR #  1 FROM BASIS VECTOR FILE # 1*/
 /*   */
*++tmpPtr =  0.1348141464E-02;    /* SAMPLE #  1*/
*++tmpPtr = -0.7456820458E-03;    /* SAMPLE #  2*/
*++tmpPtr = -0.7280433783E-03;    /* SAMPLE #  3*/
*++tmpPtr = -0.1097297994E-02;    /* SAMPLE #  4*/
*++tmpPtr = -0.2324175002E-03;    /* SAMPLE #  5*/
*++tmpPtr = -0.1132083591E-02;    /* SAMPLE #  6*/
*++tmpPtr = -0.7839847822E-03;    /* SAMPLE #  7*/
*++tmpPtr = -0.1025272650E-02;    /* SAMPLE #  8*/
*++tmpPtr = -0.9059304721E-03;    /* SAMPLE #  9*/
*++tmpPtr = -0.7070312276E-03;    /* SAMPLE # 10*/
*++tmpPtr =  0.1480115316E-03;    /* SAMPLE # 11*/
*++tmpPtr = -0.8389785071E-03;    /* SAMPLE # 12*/
*++tmpPtr =  0.4850777332E-03;    /* SAMPLE # 13*/
*++tmpPtr = -0.4760023439E-03;    /* SAMPLE # 14*/
*++tmpPtr =  0.9217599290E-04;    /* SAMPLE # 15*/
*++tmpPtr =  0.1059864881E-02;    /* SAMPLE # 16*/
*++tmpPtr =  0.6377621321E-03;    /* SAMPLE # 17*/
*++tmpPtr = -0.1941565133E-03;    /* SAMPLE # 18*/
*++tmpPtr =  0.7432991988E-03;    /* SAMPLE # 19*/
*++tmpPtr = -0.5215679266E-04;    /* SAMPLE # 20*/
*++tmpPtr =  0.3603060322E-03;    /* SAMPLE # 21*/
*++tmpPtr = -0.2318393381E-03;    /* SAMPLE # 22*/
*++tmpPtr =  0.7940563955E-04;    /* SAMPLE # 23*/
*++tmpPtr = -0.8093074430E-03;    /* SAMPLE # 24*/
*++tmpPtr =  0.8187464118E-04;    /* SAMPLE # 25*/
*++tmpPtr = -0.6052269600E-03;    /* SAMPLE # 26*/
*++tmpPtr = -0.8361758082E-03;    /* SAMPLE # 27*/
*++tmpPtr = -0.9101281175E-03;    /* SAMPLE # 28*/
*++tmpPtr = -0.1269274362E-03;    /* SAMPLE # 29*/
*++tmpPtr = -0.4765875055E-03;    /* SAMPLE # 30*/
*++tmpPtr =  0.7068230188E-03;    /* SAMPLE # 31*/
*++tmpPtr =  0.2633952245E-03;    /* SAMPLE # 32*/
*++tmpPtr =  0.4670570197E-03;    /* SAMPLE # 33*/
*++tmpPtr =  0.1561889425E-02;    /* SAMPLE # 34*/
*++tmpPtr =  0.1074319123E-02;    /* SAMPLE # 35*/
*++tmpPtr =  0.5456429790E-03;    /* SAMPLE # 36*/
*++tmpPtr = -0.8847222780E-03;    /* SAMPLE # 37*/
*++tmpPtr =  0.9608042892E-03;    /* SAMPLE # 38*/
*++tmpPtr = -0.5775320460E-03;    /* SAMPLE # 39*/
*++tmpPtr =  0.5271988339E-03;    /* SAMPLE # 40*/
 /*   */
 /* BASIS VECTOR #  2 FROM BASIS VECTOR FILE # 1*/
 /*   */
*++tmpPtr = -0.6442472804E-03;    /* SAMPLE #  1*/
*++tmpPtr = -0.5002476391E-03;    /* SAMPLE #  2*/
*++tmpPtr =  0.8143195300E-03;    /* SAMPLE #  3*/
*++tmpPtr =  0.1131742465E-03;    /* SAMPLE #  4*/
*++tmpPtr = -0.4802035110E-03;    /* SAMPLE #  5*/
*++tmpPtr =  0.3805742308E-03;    /* SAMPLE #  6*/
*++tmpPtr =  0.2169326472E-03;    /* SAMPLE #  7*/
*++tmpPtr =  0.1286984771E-02;    /* SAMPLE #  8*/
*++tmpPtr = -0.5259214668E-03;    /* SAMPLE #  9*/
*++tmpPtr =  0.4908082192E-03;    /* SAMPLE # 10*/
*++tmpPtr = -0.4564203264E-03;    /* SAMPLE # 11*/
*++tmpPtr =  0.2434299313E-04;    /* SAMPLE # 12*/
*++tmpPtr =  0.3106768709E-03;    /* SAMPLE # 13*/
*++tmpPtr =  0.1247571432E-02;    /* SAMPLE # 14*/
*++tmpPtr = -0.1072661369E-02;    /* SAMPLE # 15*/
*++tmpPtr = -0.5325021266E-04;    /* SAMPLE # 16*/
*++tmpPtr = -0.6899363361E-03;    /* SAMPLE # 17*/
*++tmpPtr = -0.7492913865E-03;    /* SAMPLE # 18*/
*++tmpPtr = -0.6821679417E-03;    /* SAMPLE # 19*/
*++tmpPtr = -0.8084445144E-03;    /* SAMPLE # 20*/
*++tmpPtr = -0.6133617135E-03;    /* SAMPLE # 21*/
*++tmpPtr = -0.1046609716E-02;    /* SAMPLE # 22*/
*++tmpPtr = -0.8205579798E-04;    /* SAMPLE # 23*/
*++tmpPtr = -0.7937488263E-03;    /* SAMPLE # 24*/
*++tmpPtr = -0.7987093995E-03;    /* SAMPLE # 25*/
*++tmpPtr = -0.1892354521E-04;    /* SAMPLE # 26*/
*++tmpPtr = -0.7111308514E-03;    /* SAMPLE # 27*/
*++tmpPtr = -0.2743637597E-03;    /* SAMPLE # 28*/
*++tmpPtr = -0.2031610202E-03;    /* SAMPLE # 29*/
*++tmpPtr = -0.4830015532E-03;    /* SAMPLE # 30*/
*++tmpPtr =  0.3933078260E-03;    /* SAMPLE # 31*/
*++tmpPtr = -0.1587166451E-02;    /* SAMPLE # 32*/
*++tmpPtr =  0.5135972751E-03;    /* SAMPLE # 33*/
*++tmpPtr = -0.6651334388E-05;    /* SAMPLE # 34*/
*++tmpPtr =  0.1503678854E-02;    /* SAMPLE # 35*/
*++tmpPtr =  0.1469462528E-02;    /* SAMPLE # 36*/
*++tmpPtr =  0.7158417720E-03;    /* SAMPLE # 37*/
*++tmpPtr =  0.8809496067E-03;    /* SAMPLE # 38*/
*++tmpPtr =  0.1543851569E-02;    /* SAMPLE # 39*/
*++tmpPtr =  0.7676972891E-03;    /* SAMPLE # 40*/
 /*   */
 /* BASIS VECTOR #  3 FROM BASIS VECTOR FILE # 1*/
 /*   */
*++tmpPtr =  0.4774460976E-03;    /* SAMPLE #  1*/
*++tmpPtr = -0.9545802459E-04;    /* SAMPLE #  2*/
*++tmpPtr =  0.3719168599E-03;    /* SAMPLE #  3*/
*++tmpPtr =  0.8012695471E-03;    /* SAMPLE #  4*/
*++tmpPtr = -0.1034273067E-02;    /* SAMPLE #  5*/
*++tmpPtr = -0.1684093731E-03;    /* SAMPLE #  6*/
*++tmpPtr = -0.1071975334E-02;    /* SAMPLE #  7*/
*++tmpPtr =  0.1885056408E-03;    /* SAMPLE #  8*/
*++tmpPtr = -0.5632484099E-03;    /* SAMPLE #  9*/
*++tmpPtr =  0.6935734418E-03;    /* SAMPLE # 10*/
*++tmpPtr =  0.8701402112E-03;    /* SAMPLE # 11*/
*++tmpPtr =  0.1640435541E-02;    /* SAMPLE # 12*/
*++tmpPtr =  0.1331149950E-02;    /* SAMPLE # 13*/
*++tmpPtr =  0.1236294163E-02;    /* SAMPLE # 14*/
*++tmpPtr =  0.5449445453E-03;    /* SAMPLE # 15*/
*++tmpPtr = -0.8010320598E-03;    /* SAMPLE # 16*/
*++tmpPtr =  0.2522510767E-03;    /* SAMPLE # 17*/
*++tmpPtr = -0.1381986775E-02;    /* SAMPLE # 18*/
*++tmpPtr = -0.2338050981E-03;    /* SAMPLE # 19*/
*++tmpPtr = -0.1148664043E-02;    /* SAMPLE # 20*/
*++tmpPtr =  0.6423383020E-03;    /* SAMPLE # 21*/
*++tmpPtr = -0.3793983778E-03;    /* SAMPLE # 22*/
*++tmpPtr =  0.9160812479E-03;    /* SAMPLE # 23*/
*++tmpPtr = -0.4457050818E-03;    /* SAMPLE # 24*/
*++tmpPtr =  0.1268393011E-02;    /* SAMPLE # 25*/
*++tmpPtr = -0.2757513721E-03;    /* SAMPLE # 26*/
*++tmpPtr = -0.5732171703E-03;    /* SAMPLE # 27*/
*++tmpPtr = -0.4271566286E-03;    /* SAMPLE # 28*/
*++tmpPtr =  0.5040742108E-03;    /* SAMPLE # 29*/
*++tmpPtr = -0.1198208542E-02;    /* SAMPLE # 30*/
*++tmpPtr =  0.2728783875E-03;    /* SAMPLE # 31*/
*++tmpPtr =  0.1320361742E-02;    /* SAMPLE # 32*/
*++tmpPtr =  0.4143363331E-03;    /* SAMPLE # 33*/
*++tmpPtr =  0.1645684824E-04;    /* SAMPLE # 34*/
*++tmpPtr =  0.9879128775E-03;    /* SAMPLE # 35*/
*++tmpPtr = -0.2076724741E-04;    /* SAMPLE # 36*/
*++tmpPtr = -0.3667873680E-03;    /* SAMPLE # 37*/
*++tmpPtr = -0.5501476699E-03;    /* SAMPLE # 38*/
*++tmpPtr = -0.7598171942E-03;    /* SAMPLE # 39*/
*++tmpPtr = -0.1487325062E-03;    /* SAMPLE # 40*/
 /*   */
 /* BASIS VECTOR #  4 FROM BASIS VECTOR FILE # 1*/
 /*   */
*++tmpPtr = -0.7822082262E-03;    /* SAMPLE #  1*/
*++tmpPtr =  0.1209011767E-02;    /* SAMPLE #  2*/
*++tmpPtr =  0.4661300918E-03;    /* SAMPLE #  3*/
*++tmpPtr =  0.9773293277E-03;    /* SAMPLE #  4*/
*++tmpPtr =  0.2868954325E-03;    /* SAMPLE #  5*/
*++tmpPtr =  0.1130871824E-02;    /* SAMPLE #  6*/
*++tmpPtr =  0.1929793361E-03;    /* SAMPLE #  7*/
*++tmpPtr = -0.6534542772E-03;    /* SAMPLE #  8*/
*++tmpPtr =  0.6879874854E-03;    /* SAMPLE #  9*/
*++tmpPtr =  0.2867876319E-03;    /* SAMPLE # 10*/
*++tmpPtr = -0.6957995938E-03;    /* SAMPLE # 11*/
*++tmpPtr =  0.1713329257E-03;    /* SAMPLE # 12*/
*++tmpPtr = -0.1820161124E-03;    /* SAMPLE # 13*/
*++tmpPtr = -0.2833442704E-03;    /* SAMPLE # 14*/
*++tmpPtr = -0.1135444967E-02;    /* SAMPLE # 15*/
*++tmpPtr = -0.2433381014E-03;    /* SAMPLE # 16*/
*++tmpPtr = -0.5116687389E-03;    /* SAMPLE # 17*/
*++tmpPtr =  0.2991441579E-03;    /* SAMPLE # 18*/
*++tmpPtr =  0.6812195061E-03;    /* SAMPLE # 19*/
*++tmpPtr =  0.7246308378E-03;    /* SAMPLE # 20*/
*++tmpPtr =  0.1376502682E-02;    /* SAMPLE # 21*/
*++tmpPtr =  0.4478433693E-03;    /* SAMPLE # 22*/
*++tmpPtr =  0.6210523425E-03;    /* SAMPLE # 23*/
*++tmpPtr =  0.8680468891E-03;    /* SAMPLE # 24*/
*++tmpPtr = -0.4403886487E-04;    /* SAMPLE # 25*/
*++tmpPtr = -0.1047628117E-02;    /* SAMPLE # 26*/
*++tmpPtr = -0.1430639066E-02;    /* SAMPLE # 27*/
*++tmpPtr = -0.1452277764E-02;    /* SAMPLE # 28*/
*++tmpPtr =  0.1168587551E-03;    /* SAMPLE # 29*/
*++tmpPtr = -0.1380463247E-02;    /* SAMPLE # 30*/
*++tmpPtr = -0.1651119819E-03;    /* SAMPLE # 31*/
*++tmpPtr =  0.2628665534E-03;    /* SAMPLE # 32*/
*++tmpPtr = -0.1293604291E-03;    /* SAMPLE # 33*/
*++tmpPtr =  0.1401925343E-02;    /* SAMPLE # 34*/
*++tmpPtr = -0.9050925728E-03;    /* SAMPLE # 35*/
*++tmpPtr =  0.4339234438E-03;    /* SAMPLE # 36*/
*++tmpPtr = -0.4097324272E-03;    /* SAMPLE # 37*/
*++tmpPtr =  0.2657096193E-03;    /* SAMPLE # 38*/
*++tmpPtr =  0.1388154196E-03;    /* SAMPLE # 39*/
*++tmpPtr =  0.1083192401E-03;    /* SAMPLE # 40*/
 /*   */
 /* BASIS VECTOR #  5 FROM BASIS VECTOR FILE # 1*/
 /*   */
*++tmpPtr = -0.2532672370E-02;    /* SAMPLE #  1*/
*++tmpPtr = -0.5748358308E-04;    /* SAMPLE #  2*/
*++tmpPtr = -0.1533641829E-02;    /* SAMPLE #  3*/
*++tmpPtr = -0.1518826175E-03;    /* SAMPLE #  4*/
*++tmpPtr =  0.8123111184E-04;    /* SAMPLE #  5*/
*++tmpPtr =  0.7646668237E-03;    /* SAMPLE #  6*/
*++tmpPtr = -0.3862913291E-03;    /* SAMPLE #  7*/
*++tmpPtr =  0.1494152355E-02;    /* SAMPLE #  8*/
*++tmpPtr =  0.3296583600E-03;    /* SAMPLE #  9*/
*++tmpPtr =  0.1173227676E-02;    /* SAMPLE # 10*/
*++tmpPtr =  0.6909203366E-03;    /* SAMPLE # 11*/
*++tmpPtr =  0.8859495865E-03;    /* SAMPLE # 12*/
*++tmpPtr =  0.1500042621E-02;    /* SAMPLE # 13*/
*++tmpPtr = -0.4756593844E-03;    /* SAMPLE # 14*/
*++tmpPtr =  0.5682234187E-03;    /* SAMPLE # 15*/
*++tmpPtr = -0.2026686707E-03;    /* SAMPLE # 16*/
*++tmpPtr =  0.4457800824E-03;    /* SAMPLE # 17*/
*++tmpPtr = -0.9680401854E-04;    /* SAMPLE # 18*/
*++tmpPtr =  0.3748453455E-03;    /* SAMPLE # 19*/
*++tmpPtr =  0.5596519331E-03;    /* SAMPLE # 20*/
*++tmpPtr = -0.2114801755E-03;    /* SAMPLE # 21*/
*++tmpPtr =  0.1542037353E-04;    /* SAMPLE # 22*/
*++tmpPtr = -0.1010505133E-02;    /* SAMPLE # 23*/
*++tmpPtr = -0.8163056918E-04;    /* SAMPLE # 24*/
*++tmpPtr = -0.8208925719E-03;    /* SAMPLE # 25*/
*++tmpPtr = -0.2370179573E-03;    /* SAMPLE # 26*/
*++tmpPtr = -0.1056237306E-03;    /* SAMPLE # 27*/
*++tmpPtr =  0.2713823633E-03;    /* SAMPLE # 28*/
*++tmpPtr =  0.5130070494E-03;    /* SAMPLE # 29*/
*++tmpPtr =  0.2483512799E-03;    /* SAMPLE # 30*/
*++tmpPtr =  0.9341922851E-04;    /* SAMPLE # 31*/
*++tmpPtr =  0.2559089917E-03;    /* SAMPLE # 32*/
*++tmpPtr =  0.2186683560E-03;    /* SAMPLE # 33*/
*++tmpPtr = -0.1874748123E-03;    /* SAMPLE # 34*/
*++tmpPtr = -0.2399277291E-03;    /* SAMPLE # 35*/
*++tmpPtr = -0.1215934753E-02;    /* SAMPLE # 36*/
*++tmpPtr = -0.1530637150E-02;    /* SAMPLE # 37*/
*++tmpPtr = -0.3211313160E-03;    /* SAMPLE # 38*/
*++tmpPtr =  0.1145689657E-04;    /* SAMPLE # 39*/
*++tmpPtr = -0.5671796971E-03;    /* SAMPLE # 40*/
 /*   */
 /* BASIS VECTOR #  6 FROM BASIS VECTOR FILE # 1*/
 /*   */
*++tmpPtr =  0.1333689084E-02;    /* SAMPLE #  1*/
*++tmpPtr =  0.9007171611E-03;    /* SAMPLE #  2*/
*++tmpPtr = -0.4508757847E-03;    /* SAMPLE #  3*/
*++tmpPtr =  0.1809281093E-03;    /* SAMPLE #  4*/
*++tmpPtr = -0.2595121623E-03;    /* SAMPLE #  5*/
*++tmpPtr =  0.3266170097E-03;    /* SAMPLE #  6*/
*++tmpPtr = -0.3657424240E-03;    /* SAMPLE #  7*/
*++tmpPtr = -0.1070841099E-03;    /* SAMPLE #  8*/
*++tmpPtr =  0.2778349444E-03;    /* SAMPLE #  9*/
*++tmpPtr = -0.4901324282E-03;    /* SAMPLE # 10*/
*++tmpPtr =  0.3027762868E-03;    /* SAMPLE # 11*/
*++tmpPtr =  0.7017797907E-04;    /* SAMPLE # 12*/
*++tmpPtr =  0.1963461749E-02;    /* SAMPLE # 13*/
*++tmpPtr =  0.3084537748E-03;    /* SAMPLE # 14*/
*++tmpPtr =  0.1671653125E-02;    /* SAMPLE # 15*/
*++tmpPtr =  0.8503804565E-03;    /* SAMPLE # 16*/
*++tmpPtr =  0.8942558779E-03;    /* SAMPLE # 17*/
*++tmpPtr =  0.1803963969E-03;    /* SAMPLE # 18*/
*++tmpPtr =  0.6471863016E-03;    /* SAMPLE # 19*/
*++tmpPtr = -0.2119630954E-03;    /* SAMPLE # 20*/
*++tmpPtr =  0.4407413944E-03;    /* SAMPLE # 21*/
*++tmpPtr = -0.1453751465E-03;    /* SAMPLE # 22*/
*++tmpPtr =  0.3023529716E-03;    /* SAMPLE # 23*/
*++tmpPtr = -0.9545351495E-03;    /* SAMPLE # 24*/
*++tmpPtr = -0.7402097690E-03;    /* SAMPLE # 25*/
*++tmpPtr =  0.5771621363E-03;    /* SAMPLE # 26*/
*++tmpPtr = -0.1970063662E-02;    /* SAMPLE # 27*/
*++tmpPtr =  0.1060343202E-03;    /* SAMPLE # 28*/
*++tmpPtr = -0.5275328294E-03;    /* SAMPLE # 29*/
*++tmpPtr = -0.1425403607E-03;    /* SAMPLE # 30*/
*++tmpPtr = -0.4066753900E-03;    /* SAMPLE # 31*/
*++tmpPtr = -0.1074151602E-02;    /* SAMPLE # 32*/
*++tmpPtr = -0.2039313840E-03;    /* SAMPLE # 33*/
*++tmpPtr = -0.1305896440E-02;    /* SAMPLE # 34*/
*++tmpPtr = -0.1352003543E-02;    /* SAMPLE # 35*/
*++tmpPtr = -0.1409178658E-03;    /* SAMPLE # 36*/
*++tmpPtr =  0.3471188538E-03;    /* SAMPLE # 37*/
*++tmpPtr = -0.9945152124E-04;    /* SAMPLE # 38*/
*++tmpPtr =  0.1673202205E-03;    /* SAMPLE # 39*/
*++tmpPtr = -0.2537889304E-04;    /* SAMPLE # 40*/
 /*   */
 /* BASIS VECTOR #  7 FROM BASIS VECTOR FILE # 1*/
 /*   */
*++tmpPtr = -0.3873030422E-02;    /* SAMPLE #  1*/
*++tmpPtr = -0.1646166493E-03;    /* SAMPLE #  2*/
*++tmpPtr = -0.8067851304E-03;    /* SAMPLE #  3*/
*++tmpPtr = -0.2398323995E-03;    /* SAMPLE #  4*/
*++tmpPtr = -0.8414450713E-04;    /* SAMPLE #  5*/
*++tmpPtr =  0.3922324104E-03;    /* SAMPLE #  6*/
*++tmpPtr =  0.7077296614E-03;    /* SAMPLE #  7*/
*++tmpPtr =  0.1954745676E-03;    /* SAMPLE #  8*/
*++tmpPtr =  0.1001161756E-02;    /* SAMPLE #  9*/
*++tmpPtr = -0.5950015620E-03;    /* SAMPLE # 10*/
*++tmpPtr = -0.1044761448E-03;    /* SAMPLE # 11*/
*++tmpPtr = -0.5065312725E-03;    /* SAMPLE # 12*/
*++tmpPtr =  0.1929078280E-03;    /* SAMPLE # 13*/
*++tmpPtr =  0.4148316511E-03;    /* SAMPLE # 14*/
*++tmpPtr =  0.7778933505E-03;    /* SAMPLE # 15*/
*++tmpPtr =  0.1052974607E-02;    /* SAMPLE # 16*/
*++tmpPtr = -0.7820314204E-04;    /* SAMPLE # 17*/
*++tmpPtr =  0.1135084778E-02;    /* SAMPLE # 18*/
*++tmpPtr = -0.1221738967E-04;    /* SAMPLE # 19*/
*++tmpPtr =  0.3970504040E-03;    /* SAMPLE # 20*/
*++tmpPtr =  0.5075788940E-03;    /* SAMPLE # 21*/
*++tmpPtr =  0.8996097604E-03;    /* SAMPLE # 22*/
*++tmpPtr =  0.1133735990E-02;    /* SAMPLE # 23*/
*++tmpPtr =  0.7409714744E-03;    /* SAMPLE # 24*/
*++tmpPtr =  0.1270795357E-02;    /* SAMPLE # 25*/
*++tmpPtr =  0.1009840562E-03;    /* SAMPLE # 26*/
*++tmpPtr = -0.1027774306E-04;    /* SAMPLE # 27*/
*++tmpPtr = -0.3100106842E-03;    /* SAMPLE # 28*/
*++tmpPtr = -0.3855471150E-03;    /* SAMPLE # 29*/
*++tmpPtr = -0.6478417781E-03;    /* SAMPLE # 30*/
*++tmpPtr = -0.4073894524E-03;    /* SAMPLE # 31*/
*++tmpPtr = -0.6222699303E-03;    /* SAMPLE # 32*/
*++tmpPtr =  0.2735181188E-03;    /* SAMPLE # 33*/
*++tmpPtr =  0.3578863107E-04;    /* SAMPLE # 34*/
*++tmpPtr =  0.2171033702E-04;    /* SAMPLE # 35*/
*++tmpPtr = -0.1114187326E-03;    /* SAMPLE # 36*/
*++tmpPtr =  0.1713896017E-04;    /* SAMPLE # 37*/
*++tmpPtr = -0.2900607069E-03;    /* SAMPLE # 38*/
*++tmpPtr = -0.4331984383E-03;    /* SAMPLE # 39*/
*++tmpPtr = -0.4126839922E-03;    /* SAMPLE # 40*/
 /*   */
 /* BASIS VECTOR #  1 FROM BASIS VECTOR FILE # 2*/
 /*   */
tmpPtr = BASIS_A - 1;
*++tmpPtr = -0.4212773405E-02;    /* SAMPLE #  1*/
*++tmpPtr =  0.1257884433E-02;    /* SAMPLE #  2*/
*++tmpPtr =  0.1395349624E-02;    /* SAMPLE #  3*/
*++tmpPtr =  0.2356290584E-02;    /* SAMPLE #  4*/
*++tmpPtr =  0.1285648439E-02;    /* SAMPLE #  5*/
*++tmpPtr = -0.8078804822E-03;    /* SAMPLE #  6*/
*++tmpPtr = -0.3066687786E-03;    /* SAMPLE #  7*/
*++tmpPtr =  0.3033672401E-03;    /* SAMPLE #  8*/
*++tmpPtr = -0.2163776662E-02;    /* SAMPLE #  9*/
*++tmpPtr = -0.1757691847E-02;    /* SAMPLE # 10*/
*++tmpPtr = -0.1122966059E-02;    /* SAMPLE # 11*/
*++tmpPtr =  0.4654710938E-03;    /* SAMPLE # 12*/
*++tmpPtr = -0.4976152559E-03;    /* SAMPLE # 13*/
*++tmpPtr =  0.1118163229E-02;    /* SAMPLE # 14*/
*++tmpPtr =  0.8649531519E-03;    /* SAMPLE # 15*/
*++tmpPtr =  0.1012576162E-02;    /* SAMPLE # 16*/
*++tmpPtr = -0.7665714948E-03;    /* SAMPLE # 17*/
*++tmpPtr = -0.2327820985E-03;    /* SAMPLE # 18*/
*++tmpPtr =  0.6818742258E-03;    /* SAMPLE # 19*/
*++tmpPtr = -0.3255666525E-04;    /* SAMPLE # 20*/
*++tmpPtr = -0.5479855463E-03;    /* SAMPLE # 21*/
*++tmpPtr = -0.9452722734E-03;    /* SAMPLE # 22*/
*++tmpPtr =  0.3581462661E-03;    /* SAMPLE # 23*/
*++tmpPtr = -0.9748523589E-03;    /* SAMPLE # 24*/
*++tmpPtr = -0.9451496298E-03;    /* SAMPLE # 25*/
*++tmpPtr =  0.6425117026E-03;    /* SAMPLE # 26*/
*++tmpPtr = -0.9998952737E-03;    /* SAMPLE # 27*/
*++tmpPtr =  0.2205711789E-02;    /* SAMPLE # 28*/
*++tmpPtr = -0.8219885785E-04;    /* SAMPLE # 29*/
*++tmpPtr =  0.3544166102E-03;    /* SAMPLE # 30*/
*++tmpPtr = -0.1280482393E-02;    /* SAMPLE # 31*/
*++tmpPtr =  0.9681124939E-03;    /* SAMPLE # 32*/
*++tmpPtr =  0.1153116464E-02;    /* SAMPLE # 33*/
*++tmpPtr =  0.9709628648E-03;    /* SAMPLE # 34*/
*++tmpPtr = -0.4851378908E-03;    /* SAMPLE # 35*/
*++tmpPtr = -0.2266674273E-03;    /* SAMPLE # 36*/
*++tmpPtr = -0.1416451181E-02;    /* SAMPLE # 37*/
*++tmpPtr = -0.1470442076E-03;    /* SAMPLE # 38*/
*++tmpPtr = -0.3248247667E-03;    /* SAMPLE # 39*/
*++tmpPtr = -0.5061195116E-03;    /* SAMPLE # 40*/
 /*   */
 /* BASIS VECTOR #  2 FROM BASIS VECTOR FILE # 2*/
 /*   */
*++tmpPtr = -0.2410669811E-02;    /* SAMPLE #  1*/
*++tmpPtr =  0.4072109296E-03;    /* SAMPLE #  2*/
*++tmpPtr =  0.7438578177E-03;    /* SAMPLE #  3*/
*++tmpPtr = -0.7827793015E-03;    /* SAMPLE #  4*/
*++tmpPtr =  0.9235066245E-03;    /* SAMPLE #  5*/
*++tmpPtr = -0.1535664895E-02;    /* SAMPLE #  6*/
*++tmpPtr = -0.2910780313E-03;    /* SAMPLE #  7*/
*++tmpPtr = -0.4350472591E-03;    /* SAMPLE #  8*/
*++tmpPtr = -0.1287800027E-02;    /* SAMPLE #  9*/
*++tmpPtr =  0.2150724613E-03;    /* SAMPLE # 10*/
*++tmpPtr = -0.8677550941E-03;    /* SAMPLE # 11*/
*++tmpPtr =  0.2368457848E-02;    /* SAMPLE # 12*/
*++tmpPtr =  0.7616058574E-03;    /* SAMPLE # 13*/
*++tmpPtr = -0.3906037018E-03;    /* SAMPLE # 14*/
*++tmpPtr = -0.9684425895E-03;    /* SAMPLE # 15*/
*++tmpPtr = -0.4347152426E-03;    /* SAMPLE # 16*/
*++tmpPtr =  0.2776849142E-03;    /* SAMPLE # 17*/
*++tmpPtr = -0.1221697748E-03;    /* SAMPLE # 18*/
*++tmpPtr = -0.1367083314E-04;    /* SAMPLE # 19*/
*++tmpPtr =  0.1607894897E-02;    /* SAMPLE # 20*/
*++tmpPtr =  0.2456868067E-02;    /* SAMPLE # 21*/
*++tmpPtr = -0.4933173113E-04;    /* SAMPLE # 22*/
*++tmpPtr =  0.1383244176E-02;    /* SAMPLE # 23*/
*++tmpPtr = -0.8562461007E-03;    /* SAMPLE # 24*/
*++tmpPtr = -0.1619120361E-02;    /* SAMPLE # 25*/
*++tmpPtr =  0.1257549739E-03;    /* SAMPLE # 26*/
*++tmpPtr =  0.4078562488E-03;    /* SAMPLE # 27*/
*++tmpPtr =  0.4947081907E-03;    /* SAMPLE # 28*/
*++tmpPtr =  0.1836035983E-02;    /* SAMPLE # 29*/
*++tmpPtr =  0.8084347937E-03;    /* SAMPLE # 30*/
*++tmpPtr =  0.9851382347E-03;    /* SAMPLE # 31*/
*++tmpPtr =  0.2118634584E-03;    /* SAMPLE # 32*/
*++tmpPtr = -0.6584765506E-03;    /* SAMPLE # 33*/
*++tmpPtr = -0.2327158116E-02;    /* SAMPLE # 34*/
*++tmpPtr =  0.5797183840E-03;    /* SAMPLE # 35*/
*++tmpPtr =  0.1468178933E-02;    /* SAMPLE # 36*/
*++tmpPtr =  0.1444990281E-02;    /* SAMPLE # 37*/
*++tmpPtr =  0.1025498030E-02;    /* SAMPLE # 38*/
*++tmpPtr = -0.8581292932E-03;    /* SAMPLE # 39*/
*++tmpPtr =  0.8698917809E-03;    /* SAMPLE # 40*/
 /*   */
 /* BASIS VECTOR #  3 FROM BASIS VECTOR FILE # 2*/
 /*   */
*++tmpPtr = -0.2749001142E-02;    /* SAMPLE #  1*/
*++tmpPtr =  0.6953945267E-03;    /* SAMPLE #  2*/
*++tmpPtr =  0.5415086052E-03;    /* SAMPLE #  3*/
*++tmpPtr =  0.6929313531E-04;    /* SAMPLE #  4*/
*++tmpPtr =  0.1247701119E-02;    /* SAMPLE #  5*/
*++tmpPtr = -0.3186027752E-02;    /* SAMPLE #  6*/
*++tmpPtr = -0.1457134349E-03;    /* SAMPLE #  7*/
*++tmpPtr =  0.4620070395E-03;    /* SAMPLE #  8*/
*++tmpPtr = -0.1648927719E-03;    /* SAMPLE #  9*/
*++tmpPtr =  0.3939234302E-03;    /* SAMPLE # 10*/
*++tmpPtr =  0.1341653522E-02;    /* SAMPLE # 11*/
*++tmpPtr = -0.1377443783E-02;    /* SAMPLE # 12*/
*++tmpPtr = -0.9186093812E-03;    /* SAMPLE # 13*/
*++tmpPtr = -0.8607968921E-03;    /* SAMPLE # 14*/
*++tmpPtr = -0.1241199880E-04;    /* SAMPLE # 15*/
*++tmpPtr =  0.6665746332E-03;    /* SAMPLE # 16*/
*++tmpPtr =  0.3055708949E-03;    /* SAMPLE # 17*/
*++tmpPtr = -0.1985677751E-02;    /* SAMPLE # 18*/
*++tmpPtr = -0.1008499181E-02;    /* SAMPLE # 19*/
*++tmpPtr = -0.1999235479E-02;    /* SAMPLE # 20*/
*++tmpPtr =  0.1029149164E-02;    /* SAMPLE # 21*/
*++tmpPtr =  0.1192038646E-02;    /* SAMPLE # 22*/
*++tmpPtr =  0.4774281115E-03;    /* SAMPLE # 23*/
*++tmpPtr = -0.4355093697E-03;    /* SAMPLE # 24*/
*++tmpPtr = -0.1385265612E-02;    /* SAMPLE # 25*/
*++tmpPtr = -0.3472096287E-03;    /* SAMPLE # 26*/
*++tmpPtr = -0.7731777150E-03;    /* SAMPLE # 27*/
*++tmpPtr = -0.1578416442E-02;    /* SAMPLE # 28*/
*++tmpPtr =  0.4095243276E-04;    /* SAMPLE # 29*/
*++tmpPtr =  0.2504375298E-03;    /* SAMPLE # 30*/
*++tmpPtr =  0.1074215979E-02;    /* SAMPLE # 31*/
*++tmpPtr =  0.1328480430E-02;    /* SAMPLE # 32*/
*++tmpPtr = -0.1117585780E-03;    /* SAMPLE # 33*/
*++tmpPtr = -0.1344937133E-02;    /* SAMPLE # 34*/
*++tmpPtr = -0.1577805029E-02;    /* SAMPLE # 35*/
*++tmpPtr = -0.1483787899E-02;    /* SAMPLE # 36*/
*++tmpPtr =  0.1251116046E-02;    /* SAMPLE # 37*/
*++tmpPtr = -0.8207866922E-03;    /* SAMPLE # 38*/
*++tmpPtr =  0.1664843410E-02;    /* SAMPLE # 39*/
*++tmpPtr =  0.1303671044E-02;    /* SAMPLE # 40*/
 /*   */
 /* BASIS VECTOR #  4 FROM BASIS VECTOR FILE # 2*/
 /*   */
*++tmpPtr =  0.3080106690E-03;    /* SAMPLE #  1*/
*++tmpPtr =  0.6412992370E-03;    /* SAMPLE #  2*/
*++tmpPtr =  0.1081860159E-02;    /* SAMPLE #  3*/
*++tmpPtr =  0.1226191409E-02;    /* SAMPLE #  4*/
*++tmpPtr = -0.7062562509E-03;    /* SAMPLE #  5*/
*++tmpPtr = -0.2615953097E-02;    /* SAMPLE #  6*/
*++tmpPtr =  0.9171406273E-03;    /* SAMPLE #  7*/
*++tmpPtr =  0.1729391399E-02;    /* SAMPLE #  8*/
*++tmpPtr =  0.1561155426E-02;    /* SAMPLE #  9*/
*++tmpPtr =  0.2147719963E-02;    /* SAMPLE # 10*/
*++tmpPtr = -0.7261054707E-03;    /* SAMPLE # 11*/
*++tmpPtr = -0.1166588627E-02;    /* SAMPLE # 12*/
*++tmpPtr =  0.3833138908E-03;    /* SAMPLE # 13*/
*++tmpPtr = -0.1642801217E-02;    /* SAMPLE # 14*/
*++tmpPtr =  0.1566672232E-02;    /* SAMPLE # 15*/
*++tmpPtr =  0.1307617349E-03;    /* SAMPLE # 16*/
*++tmpPtr =  0.9922812460E-03;    /* SAMPLE # 17*/
*++tmpPtr =  0.1683217939E-02;    /* SAMPLE # 18*/
*++tmpPtr =  0.4347757786E-03;    /* SAMPLE # 19*/
*++tmpPtr =  0.9055715054E-03;    /* SAMPLE # 20*/
*++tmpPtr = -0.1089542988E-02;    /* SAMPLE # 21*/
*++tmpPtr = -0.4085382898E-04;    /* SAMPLE # 22*/
*++tmpPtr =  0.5394832115E-03;    /* SAMPLE # 23*/
*++tmpPtr = -0.1750148018E-02;    /* SAMPLE # 24*/
*++tmpPtr =  0.3381741408E-03;    /* SAMPLE # 25*/
*++tmpPtr = -0.1165600028E-02;    /* SAMPLE # 26*/
*++tmpPtr =  0.7512453012E-03;    /* SAMPLE # 27*/
*++tmpPtr =  0.8715714794E-03;    /* SAMPLE # 28*/
*++tmpPtr =  0.2507012978E-03;    /* SAMPLE # 29*/
*++tmpPtr = -0.7089425926E-03;    /* SAMPLE # 30*/
*++tmpPtr =  0.9390070336E-03;    /* SAMPLE # 31*/
*++tmpPtr =  0.1375730935E-03;    /* SAMPLE # 32*/
*++tmpPtr =  0.1031739870E-02;    /* SAMPLE # 33*/
*++tmpPtr =  0.1011981280E-02;    /* SAMPLE # 34*/
*++tmpPtr = -0.6773336645E-04;    /* SAMPLE # 35*/
*++tmpPtr =  0.5628378713E-03;    /* SAMPLE # 36*/
*++tmpPtr =  0.2283596434E-02;    /* SAMPLE # 37*/
*++tmpPtr = -0.7693281514E-03;    /* SAMPLE # 38*/
*++tmpPtr = -0.8733496070E-03;    /* SAMPLE # 39*/
*++tmpPtr = -0.1185346860E-02;    /* SAMPLE # 40*/
 /*   */
 /* BASIS VECTOR #  5 FROM BASIS VECTOR FILE # 2*/
 /*   */
*++tmpPtr =  0.1702490699E-03;    /* SAMPLE #  1*/
*++tmpPtr = -0.2267912932E-03;    /* SAMPLE #  2*/
*++tmpPtr =  0.1758234808E-02;    /* SAMPLE #  3*/
*++tmpPtr = -0.8133343654E-03;    /* SAMPLE #  4*/
*++tmpPtr = -0.1399368746E-02;    /* SAMPLE #  5*/
*++tmpPtr = -0.1020935946E-02;    /* SAMPLE #  6*/
*++tmpPtr = -0.2634965582E-02;    /* SAMPLE #  7*/
*++tmpPtr =  0.2102663741E-02;    /* SAMPLE #  8*/
*++tmpPtr = -0.5772660006E-04;    /* SAMPLE #  9*/
*++tmpPtr = -0.2213957341E-03;    /* SAMPLE # 10*/
*++tmpPtr =  0.7417480228E-03;    /* SAMPLE # 11*/
*++tmpPtr =  0.1380887930E-02;    /* SAMPLE # 12*/
*++tmpPtr =  0.1663172588E-03;    /* SAMPLE # 13*/
*++tmpPtr = -0.7854425930E-03;    /* SAMPLE # 14*/
*++tmpPtr = -0.1518580480E-02;    /* SAMPLE # 15*/
*++tmpPtr = -0.1254262053E-02;    /* SAMPLE # 16*/
*++tmpPtr =  0.2888085437E-03;    /* SAMPLE # 17*/
*++tmpPtr =  0.1841049176E-02;    /* SAMPLE # 18*/
*++tmpPtr = -0.2776738547E-03;    /* SAMPLE # 19*/
*++tmpPtr = -0.7561187376E-03;    /* SAMPLE # 20*/
*++tmpPtr =  0.6487648352E-03;    /* SAMPLE # 21*/
*++tmpPtr = -0.1371811959E-03;    /* SAMPLE # 22*/
*++tmpPtr =  0.5538109108E-03;    /* SAMPLE # 23*/
*++tmpPtr = -0.9769434109E-03;    /* SAMPLE # 24*/
*++tmpPtr = -0.5910179461E-03;    /* SAMPLE # 25*/
*++tmpPtr =  0.4521972733E-03;    /* SAMPLE # 26*/
*++tmpPtr =  0.1755260513E-02;    /* SAMPLE # 27*/
*++tmpPtr =  0.1111741003E-03;    /* SAMPLE # 28*/
*++tmpPtr = -0.8187792846E-03;    /* SAMPLE # 29*/
*++tmpPtr = -0.1520936727E-02;    /* SAMPLE # 30*/
*++tmpPtr = -0.9273863398E-03;    /* SAMPLE # 31*/
*++tmpPtr = -0.2415992087E-02;    /* SAMPLE # 32*/
*++tmpPtr = -0.1504514483E-02;    /* SAMPLE # 33*/
*++tmpPtr =  0.1804427011E-02;    /* SAMPLE # 34*/
*++tmpPtr =  0.3633739834E-03;    /* SAMPLE # 35*/
*++tmpPtr = -0.1367421122E-02;    /* SAMPLE # 36*/
*++tmpPtr =  0.6310196477E-04;    /* SAMPLE # 37*/
*++tmpPtr = -0.6091289106E-03;    /* SAMPLE # 38*/
*++tmpPtr = -0.7073980523E-03;    /* SAMPLE # 39*/
*++tmpPtr = -0.6509601371E-03;    /* SAMPLE # 40*/
 /*   */
 /* BASIS VECTOR #  6 FROM BASIS VECTOR FILE # 2*/
 /*   */
*++tmpPtr =  0.2631459851E-02;    /* SAMPLE #  1*/
*++tmpPtr =  0.7173047052E-03;    /* SAMPLE #  2*/
*++tmpPtr = -0.4567202996E-03;    /* SAMPLE #  3*/
*++tmpPtr = -0.9316789219E-03;    /* SAMPLE #  4*/
*++tmpPtr = -0.1199632316E-03;    /* SAMPLE #  5*/
*++tmpPtr =  0.1411463250E-02;    /* SAMPLE #  6*/
*++tmpPtr =  0.1592603512E-02;    /* SAMPLE #  7*/
*++tmpPtr =  0.9217781480E-03;    /* SAMPLE #  8*/
*++tmpPtr =  0.1609489555E-03;    /* SAMPLE #  9*/
*++tmpPtr = -0.7347097853E-03;    /* SAMPLE # 10*/
*++tmpPtr = -0.6378254038E-03;    /* SAMPLE # 11*/
*++tmpPtr = -0.2670551185E-03;    /* SAMPLE # 12*/
*++tmpPtr = -0.1849992113E-03;    /* SAMPLE # 13*/
*++tmpPtr = -0.1210423216E-03;    /* SAMPLE # 14*/
*++tmpPtr =  0.4207956954E-03;    /* SAMPLE # 15*/
*++tmpPtr =  0.1117260195E-02;    /* SAMPLE # 16*/
*++tmpPtr = -0.1242892584E-02;    /* SAMPLE # 17*/
*++tmpPtr =  0.8888882585E-03;    /* SAMPLE # 18*/
*++tmpPtr = -0.2014490310E-02;    /* SAMPLE # 19*/
*++tmpPtr =  0.5812489326E-04;    /* SAMPLE # 20*/
*++tmpPtr =  0.7657070528E-03;    /* SAMPLE # 21*/
*++tmpPtr =  0.2192554297E-02;    /* SAMPLE # 22*/
*++tmpPtr = -0.8345831884E-03;    /* SAMPLE # 23*/
*++tmpPtr = -0.6730351597E-03;    /* SAMPLE # 24*/
*++tmpPtr = -0.1360399998E-02;    /* SAMPLE # 25*/
*++tmpPtr = -0.9642582154E-03;    /* SAMPLE # 26*/
*++tmpPtr = -0.9634700837E-04;    /* SAMPLE # 27*/
*++tmpPtr = -0.7967125275E-03;    /* SAMPLE # 28*/
*++tmpPtr =  0.2185695805E-02;    /* SAMPLE # 29*/
*++tmpPtr = -0.4191660264E-03;    /* SAMPLE # 30*/
*++tmpPtr = -0.1351943210E-03;    /* SAMPLE # 31*/
*++tmpPtr =  0.2009709860E-03;    /* SAMPLE # 32*/
*++tmpPtr =  0.4954851465E-03;    /* SAMPLE # 33*/
*++tmpPtr = -0.1032293309E-02;    /* SAMPLE # 34*/
*++tmpPtr =  0.2638510894E-02;    /* SAMPLE # 35*/
*++tmpPtr =  0.5394245964E-03;    /* SAMPLE # 36*/
*++tmpPtr = -0.1660457812E-02;    /* SAMPLE # 37*/
*++tmpPtr = -0.2355003962E-02;    /* SAMPLE # 38*/
*++tmpPtr = -0.7117980276E-03;    /* SAMPLE # 39*/
*++tmpPtr = -0.1136379666E-02;    /* SAMPLE # 40*/
 /*   */
 /* BASIS VECTOR #  7 FROM BASIS VECTOR FILE # 2*/
 /*   */
*++tmpPtr = -0.2711770358E-02;    /* SAMPLE #  1*/
*++tmpPtr =  0.4351386742E-03;    /* SAMPLE #  2*/
*++tmpPtr =  0.2349117538E-02;    /* SAMPLE #  3*/
*++tmpPtr =  0.7624293212E-03;    /* SAMPLE #  4*/
*++tmpPtr =  0.2005441114E-02;    /* SAMPLE #  5*/
*++tmpPtr =  0.4425472580E-03;    /* SAMPLE #  6*/
*++tmpPtr =  0.5614503752E-03;    /* SAMPLE #  7*/
*++tmpPtr = -0.9346398292E-03;    /* SAMPLE #  8*/
*++tmpPtr = -0.1746805036E-02;    /* SAMPLE #  9*/
*++tmpPtr =  0.1272766036E-02;    /* SAMPLE # 10*/
*++tmpPtr =  0.1672124257E-02;    /* SAMPLE # 11*/
*++tmpPtr =  0.7412038976E-03;    /* SAMPLE # 12*/
*++tmpPtr = -0.7003316423E-03;    /* SAMPLE # 13*/
*++tmpPtr = -0.1095367130E-02;    /* SAMPLE # 14*/
*++tmpPtr = -0.1809506211E-03;    /* SAMPLE # 15*/
*++tmpPtr =  0.1966655022E-02;    /* SAMPLE # 16*/
*++tmpPtr =  0.6434171810E-03;    /* SAMPLE # 17*/
*++tmpPtr =  0.6820840063E-03;    /* SAMPLE # 18*/
*++tmpPtr =  0.8730909904E-03;    /* SAMPLE # 19*/
*++tmpPtr = -0.1781119965E-02;    /* SAMPLE # 20*/
*++tmpPtr =  0.1196718309E-02;    /* SAMPLE # 21*/
*++tmpPtr = -0.1599975396E-02;    /* SAMPLE # 22*/
*++tmpPtr = -0.1049692626E-02;    /* SAMPLE # 23*/
*++tmpPtr = -0.1199858962E-02;    /* SAMPLE # 24*/
*++tmpPtr =  0.9627281106E-03;    /* SAMPLE # 25*/
*++tmpPtr =  0.4324761685E-03;    /* SAMPLE # 26*/
*++tmpPtr =  0.2020330867E-03;    /* SAMPLE # 27*/
*++tmpPtr = -0.6951007526E-03;    /* SAMPLE # 28*/
*++tmpPtr = -0.1753782388E-02;    /* SAMPLE # 29*/
*++tmpPtr =  0.1696094521E-02;    /* SAMPLE # 30*/
*++tmpPtr = -0.1467770664E-02;    /* SAMPLE # 31*/
*++tmpPtr =  0.2844906994E-03;    /* SAMPLE # 32*/
*++tmpPtr = -0.1813467941E-02;    /* SAMPLE # 33*/
*++tmpPtr =  0.3262141254E-03;    /* SAMPLE # 34*/
*++tmpPtr =  0.9346448933E-03;    /* SAMPLE # 35*/
*++tmpPtr =  0.1179714454E-02;    /* SAMPLE # 36*/
*++tmpPtr =  0.3923416807E-04;    /* SAMPLE # 37*/
*++tmpPtr = -0.1386266784E-02;    /* SAMPLE # 38*/
*++tmpPtr = -0.1147057861E-02;    /* SAMPLE # 39*/
*++tmpPtr = -0.3901273303E-03;    /* SAMPLE # 40*/

tmpPtr = GSP0_TABLE;
 /* THE GSP0 ERROR TERM VECTORS FOR  8 BITS FOLLOW*/
 /* ET VALUES WRITTEN WERE SCALED BY  1*/
 /*   */
 /* A COLUMN VECTOR OF 256 SCALED ERROR VECTORS FOLLOWS*/
 /* */
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #   1: */
 /* */
*tmpPtr = 0.3302603215E-01;    /* ET(1,1)*/
*++tmpPtr =  0.5839011073E-01;    /* ET(2,1)*/
*++tmpPtr =  0.4688198492E-01;    /* ET(3,1)*/
*++tmpPtr = -0.9641968063E-03;    /* ET(4,1)*/
*++tmpPtr = -0.7741629379E-03;    /* ET(5,1)*/
*++tmpPtr = -0.1368722063E-02;    /* ET(6,1)*/
*++tmpPtr = -0.2726796956E-03;    /* ET(7,1)*/
*++tmpPtr = -0.8523512515E-03;    /* ET(8,1)*/
*++tmpPtr = -0.5494800862E-03;    /* ET(9,1)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #   2: */
 /* */
*++tmpPtr =  0.5357717723E-01;    /* ET(1,2)*/
*++tmpPtr =  0.3550905734E-01;    /* ET(2,2)*/
*++tmpPtr =  0.3472279385E-01;    /* ET(3,2)*/
*++tmpPtr = -0.9512374527E-03;    /* ET(4,2)*/
*++tmpPtr = -0.9301747195E-03;    /* ET(5,2)*/
*++tmpPtr = -0.6164868246E-03;    /* ET(6,2)*/
*++tmpPtr = -0.7176285144E-03;    /* ET(7,2)*/
*++tmpPtr = -0.3152232675E-03;    /* ET(8,2)*/
*++tmpPtr = -0.3014181275E-03;    /* ET(9,2)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #   3: */
 /* */
*++tmpPtr =  0.4122427851E-01;    /* ET(1,3)*/
*++tmpPtr =  0.9614536911E-01;    /* ET(2,3)*/
*++tmpPtr =  0.3880342841E-01;    /* ET(3,3)*/
*++tmpPtr = -0.1981761772E-02;    /* ET(4,3)*/
*++tmpPtr = -0.7998217479E-03;    /* ET(5,3)*/
*++tmpPtr = -0.1865384867E-02;    /* ET(6,3)*/
*++tmpPtr = -0.4248602781E-03;    /* ET(7,3)*/
*++tmpPtr = -0.2310982905E-02;    /* ET(8,3)*/
*++tmpPtr = -0.3764265275E-03;    /* ET(9,3)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #   4: */
 /* */
*++tmpPtr =  0.7625202090E-01;    /* ET(1,4)*/
*++tmpPtr =  0.8941096067E-01;    /* ET(2,4)*/
*++tmpPtr =  0.4081022367E-01;    /* ET(3,4)*/
*++tmpPtr = -0.3408883000E-02;    /* ET(4,4)*/
*++tmpPtr = -0.1555930940E-02;    /* ET(5,4)*/
*++tmpPtr = -0.1824440435E-02;    /* ET(6,4)*/
*++tmpPtr = -0.1453592558E-02;    /* ET(7,4)*/
*++tmpPtr = -0.1998579828E-02;    /* ET(8,4)*/
*++tmpPtr = -0.4163685662E-03;    /* ET(9,4)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #   5: */
 /* */
*++tmpPtr =  0.7610448450E-01;    /* ET(1,5)*/
*++tmpPtr =  0.5125515908E-01;    /* ET(2,5)*/
*++tmpPtr =  0.8512070030E-01;    /* ET(3,5)*/
*++tmpPtr = -0.1950373524E-02;    /* ET(4,5)*/
*++tmpPtr = -0.3239033278E-02;    /* ET(5,5)*/
*++tmpPtr = -0.2181437332E-02;    /* ET(6,5)*/
*++tmpPtr = -0.1447973074E-02;    /* ET(7,5)*/
*++tmpPtr = -0.6567728124E-03;    /* ET(8,5)*/
*++tmpPtr = -0.1811383408E-02;    /* ET(9,5)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #   6: */
 /* */
*++tmpPtr =  0.7815317065E-01;    /* ET(1,6)*/
*++tmpPtr =  0.4234731570E-01;    /* ET(2,6)*/
*++tmpPtr =  0.3024308197E-01;    /* ET(3,6)*/
*++tmpPtr = -0.1654788386E-02;    /* ET(4,6)*/
*++tmpPtr = -0.1181796310E-02;    /* ET(5,6)*/
*++tmpPtr = -0.6403566222E-03;    /* ET(6,6)*/
*++tmpPtr = -0.1526979380E-02;    /* ET(7,6)*/
*++tmpPtr = -0.4483237863E-03;    /* ET(8,6)*/
*++tmpPtr = -0.2286609961E-03;    /* ET(9,6)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #   7: */
 /* */
*++tmpPtr =  0.1130165830E+00;    /* ET(1,7)*/
*++tmpPtr =  0.8396845311E-01;    /* ET(2,7)*/
*++tmpPtr =  0.7425145060E-01;    /* ET(3,7)*/
*++tmpPtr = -0.4744913895E-02;    /* ET(4,7)*/
*++tmpPtr = -0.4195822403E-02;    /* ET(5,7)*/
*++tmpPtr = -0.3117389744E-02;    /* ET(6,7)*/
*++tmpPtr = -0.3193186829E-02;    /* ET(7,7)*/
*++tmpPtr = -0.1762675354E-02;    /* ET(8,7)*/
*++tmpPtr = -0.1378319343E-02;    /* ET(9,7)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #   8: */
 /* */
*++tmpPtr =  0.1349135339E+00;    /* ET(1,8)*/
*++tmpPtr =  0.4870562628E-01;    /* ET(2,8)*/
*++tmpPtr =  0.6987784058E-01;    /* ET(3,8)*/
*++tmpPtr = -0.3285523737E-02;    /* ET(4,8)*/
*++tmpPtr = -0.4713733215E-02;    /* ET(5,8)*/
*++tmpPtr = -0.1701721922E-02;    /* ET(6,8)*/
*++tmpPtr = -0.4550415091E-02;    /* ET(7,8)*/
*++tmpPtr = -0.5930594634E-03;    /* ET(8,8)*/
*++tmpPtr = -0.1220728154E-02;    /* ET(9,8)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #   9: */
 /* */
*++tmpPtr =  0.6541237980E-01;    /* ET(1,9)*/
*++tmpPtr =  0.1016105786E+00;    /* ET(2,9)*/
*++tmpPtr =  0.9760773927E-01;    /* ET(3,9)*/
*++tmpPtr = -0.3323294455E-02;    /* ET(4,9)*/
*++tmpPtr = -0.3192377044E-02;    /* ET(5,9)*/
*++tmpPtr = -0.4958989099E-02;    /* ET(6,9)*/
*++tmpPtr = -0.1069694757E-02;    /* ET(7,9)*/
*++tmpPtr = -0.2581177279E-02;    /* ET(8,9)*/
*++tmpPtr = -0.2381817671E-02;    /* ET(9,9)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  10: */
 /* */
*++tmpPtr =  0.9310486913E-01;    /* ET(1,10)*/
*++tmpPtr =  0.7393582910E-01;    /* ET(2,10)*/
*++tmpPtr =  0.1409268975E+00;    /* ET(3,10)*/
*++tmpPtr = -0.3441892797E-02;    /* ET(4,10)*/
*++tmpPtr = -0.6560490001E-02;    /* ET(5,10)*/
*++tmpPtr = -0.5209773313E-02;    /* ET(6,10)*/
*++tmpPtr = -0.2167129191E-02;    /* ET(7,10)*/
*++tmpPtr = -0.1366626704E-02;    /* ET(8,10)*/
*++tmpPtr = -0.4965097643E-02;    /* ET(9,10)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  11: */
 /* */
*++tmpPtr =  0.1016136557E+00;    /* ET(1,11)*/
*++tmpPtr =  0.1371843070E+00;    /* ET(2,11)*/
*++tmpPtr =  0.1224565357E+00;    /* ET(3,11)*/
*++tmpPtr = -0.6969899405E-02;    /* ET(4,11)*/
*++tmpPtr = -0.6221628282E-02;    /* ET(5,11)*/
*++tmpPtr = -0.8399557322E-02;    /* ET(6,11)*/
*++tmpPtr = -0.2581333742E-02;    /* ET(7,11)*/
*++tmpPtr = -0.4704883322E-02;    /* ET(8,11)*/
*++tmpPtr = -0.3748900723E-02;    /* ET(9,11)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  12: */
 /* */
*++tmpPtr =  0.1442608088E+00;    /* ET(1,12)*/
*++tmpPtr =  0.8395608515E-01;    /* ET(2,12)*/
*++tmpPtr =  0.1437600702E+00;    /* ET(3,12)*/
*++tmpPtr = -0.6055785809E-02;    /* ET(4,12)*/
*++tmpPtr = -0.1036947127E-01;    /* ET(5,12)*/
*++tmpPtr = -0.6034765858E-02;    /* ET(6,12)*/
*++tmpPtr = -0.5202795379E-02;    /* ET(7,12)*/
*++tmpPtr = -0.1762155909E-02;    /* ET(8,12)*/
*++tmpPtr = -0.5166739225E-02;    /* ET(9,12)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  13: */
 /* */
*++tmpPtr =  0.1578074396E+00;    /* ET(1,13)*/
*++tmpPtr =  0.1100937501E+00;    /* ET(2,13)*/
*++tmpPtr =  0.8943187445E-01;    /* ET(3,13)*/
*++tmpPtr = -0.8686806075E-02;    /* ET(4,13)*/
*++tmpPtr = -0.7056507282E-02;    /* ET(5,13)*/
*++tmpPtr = -0.4922945052E-02;    /* ET(6,13)*/
*++tmpPtr = -0.6225796882E-02;    /* ET(7,13)*/
*++tmpPtr = -0.3030158347E-02;    /* ET(8,13)*/
*++tmpPtr = -0.1999514876E-02;    /* ET(9,13)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  14: */
 /* */
*++tmpPtr =  0.2023250461E+00;    /* ET(1,14)*/
*++tmpPtr =  0.8412393183E-01;    /* ET(2,14)*/
*++tmpPtr =  0.1093869209E+00;    /* ET(3,14)*/
*++tmpPtr = -0.8510189131E-02;    /* ET(4,14)*/
*++tmpPtr = -0.1106585748E-01;    /* ET(5,14)*/
*++tmpPtr = -0.4601029214E-02;    /* ET(6,14)*/
*++tmpPtr = -0.1023385674E-01;    /* ET(7,14)*/
*++tmpPtr = -0.1769209048E-02;    /* ET(8,14)*/
*++tmpPtr = -0.2991374815E-02;    /* ET(9,14)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  15: */
 /* */
*++tmpPtr =  0.2137947381E+00;    /* ET(1,15)*/
*++tmpPtr =  0.1523622870E+00;    /* ET(2,15)*/
*++tmpPtr =  0.1035353914E+00;    /* ET(3,15)*/
*++tmpPtr = -0.1628712751E-01;    /* ET(4,15)*/
*++tmpPtr = -0.1106766146E-01;    /* ET(5,15)*/
*++tmpPtr = -0.7887444459E-02;    /* ET(6,15)*/
*++tmpPtr = -0.1142704766E-01;    /* ET(7,15)*/
*++tmpPtr = -0.5803566892E-02;    /* ET(8,15)*/
*++tmpPtr = -0.2679894213E-02;    /* ET(9,15)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  16: */
 /* */
*++tmpPtr =  0.2791244090E+00;    /* ET(1,16)*/
*++tmpPtr =  0.1151091233E+00;    /* ET(2,16)*/
*++tmpPtr =  0.1247658506E+00;    /* ET(3,16)*/
*++tmpPtr = -0.1606488228E-01;    /* ET(4,16)*/
*++tmpPtr = -0.1741259545E-01;    /* ET(5,16)*/
*++tmpPtr = -0.7180843968E-02;    /* ET(6,16)*/
*++tmpPtr = -0.1947760768E-01;    /* ET(7,16)*/
*++tmpPtr = -0.3312527668E-02;    /* ET(8,16)*/
*++tmpPtr = -0.3891629167E-02;    /* ET(9,16)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  17: */
 /* */
*++tmpPtr =  0.1006376892E+00;    /* ET(1,17)*/
*++tmpPtr =  0.1798941493E+00;    /* ET(2,17)*/
*++tmpPtr =  0.7851902395E-01;    /* ET(3,17)*/
*++tmpPtr = -0.9052066132E-02;    /* ET(4,17)*/
*++tmpPtr = -0.3950986546E-02;    /* ET(5,17)*/
*++tmpPtr = -0.7062556222E-02;    /* ET(6,17)*/
*++tmpPtr = -0.2531986218E-02;    /* ET(7,17)*/
*++tmpPtr = -0.8090475574E-02;    /* ET(8,17)*/
*++tmpPtr = -0.1541309175E-02;    /* ET(9,17)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  18: */
 /* */
*++tmpPtr =  0.1264460534E+00;    /* ET(1,18)*/
*++tmpPtr =  0.1869618893E+00;    /* ET(2,18)*/
*++tmpPtr =  0.1252250224E+00;    /* ET(3,18)*/
*++tmpPtr = -0.1182029676E-01;    /* ET(4,18)*/
*++tmpPtr = -0.7917105220E-02;    /* ET(5,18)*/
*++tmpPtr = -0.1170615293E-01;    /* ET(6,18)*/
*++tmpPtr = -0.3997151274E-02;    /* ET(7,18)*/
*++tmpPtr = -0.8738687262E-02;    /* ET(8,18)*/
*++tmpPtr = -0.3920326475E-02;    /* ET(9,18)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  19: */
 /* */
*++tmpPtr =  0.6864681095E-01;    /* ET(1,19)*/
*++tmpPtr =  0.2217693031E+00;    /* ET(2,19)*/
*++tmpPtr =  0.1858957112E+00;    /* ET(3,19)*/
*++tmpPtr = -0.7611877285E-02;    /* ET(4,19)*/
*++tmpPtr = -0.6380573381E-02;    /* ET(5,19)*/
*++tmpPtr = -0.2061297931E-01;    /* ET(6,19)*/
*++tmpPtr = -0.1178096049E-02;    /* ET(7,19)*/
*++tmpPtr = -0.1229540538E-01;    /* ET(8,19)*/
*++tmpPtr = -0.8639303967E-02;    /* ET(9,19)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  20: */
 /* */
*++tmpPtr =  0.8476646990E-01;    /* ET(1,20)*/
*++tmpPtr =  0.2231163383E+00;    /* ET(2,20)*/
*++tmpPtr =  0.3299708366E+00;    /* ET(3,20)*/
*++tmpPtr = -0.9456391446E-02;    /* ET(4,20)*/
*++tmpPtr = -0.1398523059E-01;    /* ET(5,20)*/
*++tmpPtr = -0.3681094199E-01;    /* ET(6,20)*/
*++tmpPtr = -0.1796338474E-02;    /* ET(7,20)*/
*++tmpPtr = -0.1244522538E-01;    /* ET(8,20)*/
*++tmpPtr = -0.2722018771E-01;    /* ET(9,20)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  21: */
 /* */
*++tmpPtr =  0.1719719619E+00;    /* ET(1,21)*/
*++tmpPtr =  0.1281688809E+00;    /* ET(2,21)*/
*++tmpPtr =  0.1642188877E+00;    /* ET(3,21)*/
*++tmpPtr = -0.1102072652E-01;    /* ET(4,21)*/
*++tmpPtr = -0.1412052102E-01;    /* ET(5,21)*/
*++tmpPtr = -0.1052387524E-01;    /* ET(6,21)*/
*++tmpPtr = -0.7393588312E-02;    /* ET(7,21)*/
*++tmpPtr = -0.4106815904E-02;    /* ET(8,21)*/
*++tmpPtr = -0.6741960533E-02;    /* ET(9,21)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  22: */
 /* */
*++tmpPtr =  0.1762666851E+00;    /* ET(1,22)*/
*++tmpPtr =  0.9252157807E-01;    /* ET(2,22)*/
*++tmpPtr =  0.2724637687E+00;    /* ET(3,22)*/
*++tmpPtr = -0.8154235780E-02;    /* ET(4,22)*/
*++tmpPtr = -0.2401314117E-01;    /* ET(5,22)*/
*++tmpPtr = -0.1260438748E-01;    /* ET(6,22)*/
*++tmpPtr = -0.7767486386E-02;    /* ET(7,22)*/
*++tmpPtr = -0.2140060533E-02;    /* ET(8,22)*/
*++tmpPtr = -0.1855912432E-01;    /* ET(9,22)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  23: */
 /* */
*++tmpPtr =  0.2158354223E+00;    /* ET(1,23)*/
*++tmpPtr =  0.1345243901E+00;    /* ET(2,23)*/
*++tmpPtr =  0.3093664944E+00;    /* ET(3,23)*/
*++tmpPtr = -0.1451756526E-01;    /* ET(4,23)*/
*++tmpPtr = -0.3338612244E-01;    /* ET(5,23)*/
*++tmpPtr = -0.2080867067E-01;    /* ET(6,23)*/
*++tmpPtr = -0.1164623257E-01;    /* ET(7,23)*/
*++tmpPtr = -0.4524203017E-02;    /* ET(8,23)*/
*++tmpPtr = -0.2392690629E-01;    /* ET(9,23)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  24: */
 /* */
*++tmpPtr =  0.2726320922E+00;    /* ET(1,24)*/
*++tmpPtr =  0.1229937598E+00;    /* ET(2,24)*/
*++tmpPtr =  0.2066561729E+00;    /* ET(3,24)*/
*++tmpPtr = -0.1676602289E-01;    /* ET(4,24)*/
*++tmpPtr = -0.2817055210E-01;    /* ET(5,24)*/
*++tmpPtr = -0.1270871051E-01;    /* ET(6,24)*/
*++tmpPtr = -0.1858206466E-01;    /* ET(7,24)*/
*++tmpPtr = -0.3781866282E-02;    /* ET(8,24)*/
*++tmpPtr = -0.1067669410E-01;    /* ET(9,24)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  25: */
 /* */
*++tmpPtr =  0.2276945710E+00;    /* ET(1,25)*/
*++tmpPtr =  0.1822385937E+00;    /* ET(2,25)*/
*++tmpPtr =  0.1897393912E+00;    /* ET(3,25)*/
*++tmpPtr = -0.2074736916E-01;    /* ET(4,25)*/
*++tmpPtr = -0.2160131373E-01;    /* ET(5,25)*/
*++tmpPtr = -0.1728891954E-01;    /* ET(6,25)*/
*++tmpPtr = -0.1296120510E-01;    /* ET(7,25)*/
*++tmpPtr = -0.8302726783E-02;    /* ET(8,25)*/
*++tmpPtr = -0.9000258520E-02;    /* ET(9,25)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  26: */
 /* */
*++tmpPtr =  0.2767938077E+00;    /* ET(1,26)*/
*++tmpPtr =  0.2006916255E+00;    /* ET(2,26)*/
*++tmpPtr =  0.2263163030E+00;    /* ET(3,26)*/
*++tmpPtr = -0.2777509764E-01;    /* ET(4,26)*/
*++tmpPtr = -0.3132147342E-01;    /* ET(5,26)*/
*++tmpPtr = -0.2270989306E-01;    /* ET(6,26)*/
*++tmpPtr = -0.1915370114E-01;    /* ET(7,26)*/
*++tmpPtr = -0.1006928179E-01;    /* ET(8,26)*/
*++tmpPtr = -0.1280476712E-01;    /* ET(9,26)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  27: */
 /* */
*++tmpPtr =  0.2993195057E+00;    /* ET(1,27)*/
*++tmpPtr =  0.1884233803E+00;    /* ET(2,27)*/
*++tmpPtr =  0.1491745114E+00;    /* ET(3,27)*/
*++tmpPtr = -0.2819939703E-01;    /* ET(4,27)*/
*++tmpPtr = -0.2232542075E-01;    /* ET(5,27)*/
*++tmpPtr = -0.1405398361E-01;    /* ET(6,27)*/
*++tmpPtr = -0.2239803970E-01;    /* ET(7,27)*/
*++tmpPtr = -0.8875843138E-02;    /* ET(8,27)*/
*++tmpPtr = -0.5563259125E-02;    /* ET(9,27)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  28: */
 /* */
*++tmpPtr =  0.3602931798E+00;    /* ET(1,28)*/
*++tmpPtr =  0.1639294326E+00;    /* ET(2,28)*/
*++tmpPtr =  0.1231391355E+00;    /* ET(3,28)*/
*++tmpPtr = -0.2953132801E-01;    /* ET(4,28)*/
*++tmpPtr = -0.2218309417E-01;    /* ET(5,28)*/
*++tmpPtr = -0.1009306405E-01;    /* ET(6,28)*/
*++tmpPtr = -0.3245279193E-01;    /* ET(7,28)*/
*++tmpPtr = -0.6718214601E-02;    /* ET(8,28)*/
*++tmpPtr = -0.3790811636E-02;    /* ET(9,28)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  29: */
 /* */
*++tmpPtr =  0.3183083236E+00;    /* ET(1,29)*/
*++tmpPtr =  0.1452607065E+00;    /* ET(2,29)*/
*++tmpPtr =  0.2828883529E+00;    /* ET(3,29)*/
*++tmpPtr = -0.2311884612E-01;    /* ET(4,29)*/
*++tmpPtr = -0.4502285644E-01;    /* ET(5,29)*/
*++tmpPtr = -0.2054627985E-01;    /* ET(6,29)*/
*++tmpPtr = -0.2533004619E-01;    /* ET(7,29)*/
*++tmpPtr = -0.5275167990E-02;    /* ET(8,29)*/
*++tmpPtr = -0.2000645362E-01;    /* ET(9,29)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  30: */
 /* */
*++tmpPtr =  0.3912188709E+00;    /* ET(1,30)*/
*++tmpPtr =  0.1555002332E+00;    /* ET(2,30)*/
*++tmpPtr =  0.2756434083E+00;    /* ET(3,30)*/
*++tmpPtr = -0.3041731194E-01;    /* ET(4,30)*/
*++tmpPtr = -0.5391844735E-01;    /* ET(5,30)*/
*++tmpPtr = -0.2143130638E-01;    /* ET(6,30)*/
*++tmpPtr = -0.3826304898E-01;    /* ET(7,30)*/
*++tmpPtr = -0.6045080721E-02;    /* ET(8,30)*/
*++tmpPtr = -0.1899482124E-01;    /* ET(9,30)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  31: */
 /* */
*++tmpPtr =  0.4161453545E+00;    /* ET(1,31)*/
*++tmpPtr =  0.1111246571E+00;    /* ET(2,31)*/
*++tmpPtr =  0.1757590771E+00;    /* ET(3,31)*/
*++tmpPtr = -0.2312200330E-01;    /* ET(4,31)*/
*++tmpPtr = -0.3657066077E-01;    /* ET(5,31)*/
*++tmpPtr = -0.9765584022E-02;    /* ET(6,31)*/
*++tmpPtr = -0.4329423606E-01;    /* ET(7,31)*/
*++tmpPtr = -0.3087172285E-02;    /* ET(8,31)*/
*++tmpPtr = -0.7722812705E-02;    /* ET(9,31)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  32: */
 /* */
*++tmpPtr =  0.4876709580E+00;    /* ET(1,32)*/
*++tmpPtr =  0.1500190198E+00;    /* ET(2,32)*/
*++tmpPtr =  0.1405758262E+00;    /* ET(3,32)*/
*++tmpPtr = -0.3657995537E-01;    /* ET(4,32)*/
*++tmpPtr = -0.3427737206E-01;    /* ET(5,32)*/
*++tmpPtr = -0.1054452360E-01;    /* ET(6,32)*/
*++tmpPtr = -0.5945573747E-01;    /* ET(7,32)*/
*++tmpPtr = -0.5626426078E-02;    /* ET(8,32)*/
*++tmpPtr = -0.4940390587E-02;    /* ET(9,32)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  33: */
 /* */
*++tmpPtr =  0.1623707265E+00;    /* ET(1,33)*/
*++tmpPtr =  0.2284163237E+00;    /* ET(2,33)*/
*++tmpPtr =  0.1534326673E+00;    /* ET(3,33)*/
*++tmpPtr = -0.1854406297E-01;    /* ET(4,33)*/
*++tmpPtr = -0.1245648786E-01;    /* ET(5,33)*/
*++tmpPtr = -0.1752326451E-01;    /* ET(6,33)*/
*++tmpPtr = -0.6591063458E-02;    /* ET(7,33)*/
*++tmpPtr = -0.1304350421E-01;    /* ET(8,33)*/
*++tmpPtr = -0.5885396153E-02;    /* ET(9,33)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  34: */
 /* */
*++tmpPtr =  0.1541735977E+00;    /* ET(1,34)*/
*++tmpPtr =  0.2911500931E+00;    /* ET(2,34)*/
*++tmpPtr =  0.2025773525E+00;    /* ET(3,34)*/
*++tmpPtr = -0.2244382910E-01;    /* ET(4,34)*/
*++tmpPtr = -0.1561603975E-01;    /* ET(5,34)*/
*++tmpPtr = -0.2949020453E-01;    /* ET(6,34)*/
*++tmpPtr = -0.5942374468E-02;    /* ET(7,34)*/
*++tmpPtr = -0.2119209245E-01;    /* ET(8,34)*/
*++tmpPtr = -0.1025939547E-01;    /* ET(9,34)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  35: */
 /* */
*++tmpPtr =  0.1573798507E+00;    /* ET(1,35)*/
*++tmpPtr =  0.2262615710E+00;    /* ET(2,35)*/
*++tmpPtr =  0.2573153675E+00;    /* ET(3,35)*/
*++tmpPtr = -0.1780450530E-01;    /* ET(4,35)*/
*++tmpPtr = -0.2024812624E-01;    /* ET(5,35)*/
*++tmpPtr = -0.2911029011E-01;    /* ET(6,35)*/
*++tmpPtr = -0.6192103960E-02;    /* ET(7,35)*/
*++tmpPtr = -0.1279857475E-01;    /* ET(8,35)*/
*++tmpPtr = -0.1655280031E-01;    /* ET(9,35)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  36: */
 /* */
*++tmpPtr =  0.2147416621E+00;    /* ET(1,36)*/
*++tmpPtr =  0.2412867099E+00;    /* ET(2,36)*/
*++tmpPtr =  0.2995141745E+00;    /* ET(3,36)*/
*++tmpPtr = -0.2590715513E-01;    /* ET(4,36)*/
*++tmpPtr = -0.3215908632E-01;    /* ET(5,36)*/
*++tmpPtr = -0.3613439575E-01;    /* ET(6,36)*/
*++tmpPtr = -0.1152849570E-01;    /* ET(7,36)*/
*++tmpPtr = -0.1455481909E-01;    /* ET(8,36)*/
*++tmpPtr = -0.2242718451E-01;    /* ET(9,36)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  37: */
 /* */
*++tmpPtr =  0.1149631143E+00;    /* ET(1,37)*/
*++tmpPtr =  0.3383139074E+00;    /* ET(2,37)*/
*++tmpPtr =  0.3388412297E+00;    /* ET(3,37)*/
*++tmpPtr = -0.1944680884E-01;    /* ET(4,37)*/
*++tmpPtr = -0.1947712153E-01;    /* ET(5,37)*/
*++tmpPtr = -0.5731735379E-01;    /* ET(6,37)*/
*++tmpPtr = -0.3304129234E-02;    /* ET(7,37)*/
*++tmpPtr = -0.2861407585E-01;    /* ET(8,37)*/
*++tmpPtr = -0.2870334685E-01;    /* ET(9,37)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  38: */
 /* */
*++tmpPtr =  0.1381669939E+00;    /* ET(1,38)*/
*++tmpPtr =  0.3281638622E+00;    /* ET(2,38)*/
*++tmpPtr =  0.5633460283E+00;    /* ET(3,38)*/
*++tmpPtr = -0.2267070673E-01;    /* ET(4,38)*/
*++tmpPtr = -0.3891791403E-01;    /* ET(5,38)*/
*++tmpPtr = -0.9243490547E-01;    /* ET(6,38)*/
*++tmpPtr = -0.4772529472E-02;    /* ET(7,38)*/
*++tmpPtr = -0.2692288160E-01;    /* ET(8,38)*/
*++tmpPtr = -0.7933969051E-01;    /* ET(9,38)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  39: */
 /* */
*++tmpPtr =  0.2672561407E+00;    /* ET(1,39)*/
*++tmpPtr =  0.3067956269E+00;    /* ET(2,39)*/
*++tmpPtr =  0.3616929650E+00;    /* ET(3,39)*/
*++tmpPtr = -0.4099651054E-01;    /* ET(4,39)*/
*++tmpPtr = -0.4833233729E-01;    /* ET(5,39)*/
*++tmpPtr = -0.5548291281E-01;    /* ET(6,39)*/
*++tmpPtr = -0.1785646193E-01;    /* ET(7,39)*/
*++tmpPtr = -0.2353088930E-01;    /* ET(8,39)*/
*++tmpPtr = -0.3270545229E-01;    /* ET(9,39)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  40: */
 /* */
*++tmpPtr =  0.3122857213E+00;    /* ET(1,40)*/
*++tmpPtr =  0.2602393627E+00;    /* ET(2,40)*/
*++tmpPtr =  0.5250964761E+00;    /* ET(3,40)*/
*++tmpPtr = -0.4063451663E-01;    /* ET(4,40)*/
*++tmpPtr = -0.8199006319E-01;    /* ET(5,40)*/
*++tmpPtr = -0.6832539290E-01;    /* ET(6,40)*/
*++tmpPtr = -0.2438059263E-01;    /* ET(7,40)*/
*++tmpPtr = -0.1693113148E-01;    /* ET(8,40)*/
*++tmpPtr = -0.6893157214E-01;    /* ET(9,40)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  41: */
 /* */
*++tmpPtr =  0.2332850844E+00;    /* ET(1,41)*/
*++tmpPtr =  0.2952555716E+00;    /* ET(2,41)*/
*++tmpPtr =  0.1460955143E+00;    /* ET(3,41)*/
*++tmpPtr = -0.3443935886E-01;    /* ET(4,41)*/
*++tmpPtr = -0.1704095118E-01;    /* ET(5,41)*/
*++tmpPtr = -0.2156775631E-01;    /* ET(6,41)*/
*++tmpPtr = -0.1360548194E-01;    /* ET(7,41)*/
*++tmpPtr = -0.2179396152E-01;    /* ET(8,41)*/
*++tmpPtr = -0.5335974507E-02;    /* ET(9,41)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  42: */
 /* */
*++tmpPtr =  0.2746296823E+00;    /* ET(1,42)*/
*++tmpPtr =  0.2834746838E+00;    /* ET(2,42)*/
*++tmpPtr =  0.2198548913E+00;    /* ET(3,42)*/
*++tmpPtr = -0.3892528266E-01;    /* ET(4,42)*/
*++tmpPtr = -0.3018933907E-01;    /* ET(5,42)*/
*++tmpPtr = -0.3116164915E-01;    /* ET(6,42)*/
*++tmpPtr = -0.1885536686E-01;    /* ET(7,42)*/
*++tmpPtr = -0.2008947544E-01;    /* ET(8,42)*/
*++tmpPtr = -0.1208404358E-01;    /* ET(9,42)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  43: */
 /* */
*++tmpPtr =  0.2948631644E+00;    /* ET(1,43)*/
*++tmpPtr =  0.2195236832E+00;    /* ET(2,43)*/
*++tmpPtr =  0.3035912812E+00;    /* ET(3,43)*/
*++tmpPtr = -0.3236472607E-01;    /* ET(4,43)*/
*++tmpPtr = -0.4475894570E-01;    /* ET(5,43)*/
*++tmpPtr = -0.3332274035E-01;    /* ET(6,43)*/
*++tmpPtr = -0.2173607238E-01;    /* ET(7,43)*/
*++tmpPtr = -0.1204766240E-01;    /* ET(8,43)*/
*++tmpPtr = -0.2304191701E-01;    /* ET(9,43)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  44: */
 /* */
*++tmpPtr =  0.3643011749E+00;    /* ET(1,44)*/
*++tmpPtr =  0.2603221536E+00;    /* ET(2,44)*/
*++tmpPtr =  0.3048623204E+00;    /* ET(3,44)*/
*++tmpPtr = -0.4741783440E-01;    /* ET(4,44)*/
*++tmpPtr = -0.5553084984E-01;    /* ET(5,44)*/
*++tmpPtr = -0.3968120366E-01;    /* ET(6,44)*/
*++tmpPtr = -0.3317883611E-01;    /* ET(7,44)*/
*++tmpPtr = -0.1694190502E-01;    /* ET(8,44)*/
*++tmpPtr = -0.2323525771E-01;    /* ET(9,44)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  45: */
 /* */
*++tmpPtr =  0.3561379910E+00;    /* ET(1,45)*/
*++tmpPtr =  0.2513873279E+00;    /* ET(2,45)*/
*++tmpPtr =  0.1795235127E+00;    /* ET(3,45)*/
*++tmpPtr = -0.4476428777E-01;    /* ET(4,45)*/
*++tmpPtr = -0.3196756914E-01;    /* ET(5,45)*/
*++tmpPtr = -0.2256496809E-01;    /* ET(6,45)*/
*++tmpPtr = -0.3170856461E-01;    /* ET(7,45)*/
*++tmpPtr = -0.1579889841E-01;    /* ET(8,45)*/
*++tmpPtr = -0.8057172410E-02;    /* ET(9,45)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  46: */
 /* */
*++tmpPtr =  0.4168189168E+00;    /* ET(1,46)*/
*++tmpPtr =  0.2691484988E+00;    /* ET(2,46)*/
*++tmpPtr =  0.1690796614E+00;    /* ET(3,46)*/
*++tmpPtr = -0.5609309301E-01;    /* ET(4,46)*/
*++tmpPtr = -0.3523780406E-01;    /* ET(5,46)*/
*++tmpPtr = -0.2275376767E-01;    /* ET(6,46)*/
*++tmpPtr = -0.4343450442E-01;    /* ET(7,46)*/
*++tmpPtr = -0.1811022684E-01;    /* ET(8,46)*/
*++tmpPtr = -0.7146982942E-02;    /* ET(9,46)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  47: */
 /* */
*++tmpPtr =  0.4636695385E+00;    /* ET(1,47)*/
*++tmpPtr =  0.2486542761E+00;    /* ET(2,47)*/
*++tmpPtr =  0.2453047484E+00;    /* ET(3,47)*/
*++tmpPtr = -0.5764671043E-01;    /* ET(4,47)*/
*++tmpPtr = -0.5687017366E-01;    /* ET(5,47)*/
*++tmpPtr = -0.3049803898E-01;    /* ET(6,47)*/
*++tmpPtr = -0.5374736339E-01;    /* ET(7,47)*/
*++tmpPtr = -0.1545723807E-01;    /* ET(8,47)*/
*++tmpPtr = -0.1504360512E-01;    /* ET(9,47)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  48: */
 /* */
*++tmpPtr =  0.5315957069E+00;    /* ET(1,48)*/
*++tmpPtr =  0.2413866669E+00;    /* ET(2,48)*/
*++tmpPtr =  0.1795418113E+00;    /* ET(3,48)*/
*++tmpPtr = -0.6416005641E-01;    /* ET(4,48)*/
*++tmpPtr = -0.4772182554E-01;    /* ET(5,48)*/
*++tmpPtr = -0.2166949958E-01;    /* ET(6,48)*/
*++tmpPtr = -0.7064849138E-01;    /* ET(7,48)*/
*++tmpPtr = -0.1456688158E-01;    /* ET(8,48)*/
*++tmpPtr = -0.8058815263E-02;    /* ET(9,48)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  49: */
 /* */
*++tmpPtr =  0.3853163421E+00;    /* ET(1,49)*/
*++tmpPtr =  0.1911047995E+00;    /* ET(2,49)*/
*++tmpPtr =  0.4012880325E+00;    /* ET(3,49)*/
*++tmpPtr = -0.3681790084E-01;    /* ET(4,49)*/
*++tmpPtr = -0.7731141895E-01;    /* ET(5,49)*/
*++tmpPtr = -0.3834403306E-01;    /* ET(6,49)*/
*++tmpPtr = -0.3711717203E-01;    /* ET(7,49)*/
*++tmpPtr = -0.9130260907E-02;    /* ET(8,49)*/
*++tmpPtr = -0.4025802016E-01;    /* ET(9,49)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  50: */
 /* */
*++tmpPtr =  0.4382510185E+00;    /* ET(1,50)*/
*++tmpPtr =  0.2523638308E+00;    /* ET(2,50)*/
*++tmpPtr =  0.3685141802E+00;    /* ET(3,50)*/
*++tmpPtr = -0.5529934540E-01;    /* ET(4,50)*/
*++tmpPtr = -0.8075085282E-01;    /* ET(5,50)*/
*++tmpPtr = -0.4649981856E-01;    /* ET(6,50)*/
*++tmpPtr = -0.4801598564E-01;    /* ET(7,50)*/
*++tmpPtr = -0.1592187397E-01;    /* ET(8,50)*/
*++tmpPtr = -0.3395067528E-01;    /* ET(9,50)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  51: */
 /* */
*++tmpPtr =  0.5056229830E+00;    /* ET(1,51)*/
*++tmpPtr =  0.1714334935E+00;    /* ET(2,51)*/
*++tmpPtr =  0.4045678377E+00;    /* ET(3,51)*/
*++tmpPtr = -0.4334035888E-01;    /* ET(4,51)*/
*++tmpPtr = -0.1022794098E+00;    /* ET(5,51)*/
*++tmpPtr = -0.3467823938E-01;    /* ET(6,51)*/
*++tmpPtr = -0.6391365081E-01;    /* ET(7,51)*/
*++tmpPtr = -0.7347360719E-02;    /* ET(8,51)*/
*++tmpPtr = -0.4091878608E-01;    /* ET(9,51)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  52: */
 /* */
*++tmpPtr =  0.5729022026E+00;    /* ET(1,52)*/
*++tmpPtr =  0.1997966170E+00;    /* ET(2,52)*/
*++tmpPtr =  0.3726274073E+00;    /* ET(3,52)*/
*++tmpPtr = -0.5723196268E-01;    /* ET(4,52)*/
*++tmpPtr = -0.1067395285E+00;    /* ET(5,52)*/
*++tmpPtr = -0.3722484782E-01;    /* ET(6,52)*/
*++tmpPtr = -0.8205422759E-01;    /* ET(7,52)*/
*++tmpPtr = -0.9979672730E-02;    /* ET(8,52)*/
*++tmpPtr = -0.3471279889E-01;    /* ET(9,52)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  53: */
 /* */
*++tmpPtr =  0.4432192147E+00;    /* ET(1,53)*/
*++tmpPtr =  0.3184359670E+00;    /* ET(2,53)*/
*++tmpPtr =  0.2893881500E+00;    /* ET(3,53)*/
*++tmpPtr = -0.7056847215E-01;    /* ET(4,53)*/
*++tmpPtr = -0.6413119286E-01;    /* ET(5,53)*/
*++tmpPtr = -0.4607579857E-01;    /* ET(6,53)*/
*++tmpPtr = -0.4911081865E-01;    /* ET(7,53)*/
*++tmpPtr = -0.2535036579E-01;    /* ET(8,53)*/
*++tmpPtr = -0.2093637548E-01;    /* ET(9,53)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  54: */
 /* */
*++tmpPtr =  0.5164901614E+00;    /* ET(1,54)*/
*++tmpPtr =  0.3184528053E+00;    /* ET(2,54)*/
*++tmpPtr =  0.3138097227E+00;    /* ET(3,54)*/
*++tmpPtr = -0.8223886788E-01;    /* ET(4,54)*/
*++tmpPtr = -0.8103981614E-01;    /* ET(5,54)*/
*++tmpPtr = -0.4996678978E-01;    /* ET(6,54)*/
*++tmpPtr = -0.6669051945E-01;    /* ET(7,54)*/
*++tmpPtr = -0.2535304613E-01;    /* ET(8,54)*/
*++tmpPtr = -0.2461913414E-01;    /* ET(9,54)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  55: */
 /* */
*++tmpPtr =  0.5192453265E+00;    /* ET(1,55)*/
*++tmpPtr =  0.3461526334E+00;    /* ET(2,55)*/
*++tmpPtr =  0.1906679273E+00;    /* ET(3,55)*/
*++tmpPtr = -0.8986906707E-01;    /* ET(4,55)*/
*++tmpPtr = -0.4950171337E-01;    /* ET(5,55)*/
*++tmpPtr = -0.3300010040E-01;    /* ET(6,55)*/
*++tmpPtr = -0.6740392745E-01;    /* ET(7,55)*/
*++tmpPtr = -0.2995540947E-01;    /* ET(8,55)*/
*++tmpPtr = -0.9088564664E-02;    /* ET(9,55)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  56: */
 /* */
*++tmpPtr =  0.6507154107E+00;    /* ET(1,56)*/
*++tmpPtr =  0.3052194417E+00;    /* ET(2,56)*/
*++tmpPtr =  0.1776292026E+00;    /* ET(3,56)*/
*++tmpPtr = -0.9930551052E-01;    /* ET(4,56)*/
*++tmpPtr = -0.5779302865E-01;    /* ET(5,56)*/
*++tmpPtr = -0.2710794285E-01;    /* ET(6,56)*/
*++tmpPtr = -0.1058576405E+00;    /* ET(7,56)*/
*++tmpPtr = -0.2328972891E-01;    /* ET(8,56)*/
*++tmpPtr = -0.7888033055E-02;    /* ET(9,56)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  57: */
 /* */
*++tmpPtr =  0.4624176621E+00;    /* ET(1,57)*/
*++tmpPtr =  0.1777096689E+00;    /* ET(2,57)*/
*++tmpPtr =  0.2705087066E+00;    /* ET(3,57)*/
*++tmpPtr = -0.4108804092E-01;    /* ET(4,57)*/
*++tmpPtr = -0.6254400313E-01;    /* ET(5,57)*/
*++tmpPtr = -0.2403600700E-01;    /* ET(6,57)*/
*++tmpPtr = -0.5345752463E-01;    /* ET(7,57)*/
*++tmpPtr = -0.7895180956E-02;    /* ET(8,57)*/
*++tmpPtr = -0.1829374023E-01;    /* ET(9,57)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  58: */
 /* */
*++tmpPtr =  0.5367279053E+00;    /* ET(1,58)*/
*++tmpPtr =  0.1604519635E+00;    /* ET(2,58)*/
*++tmpPtr =  0.2397001535E+00;    /* ET(3,58)*/
*++tmpPtr = -0.4305952415E-01;    /* ET(4,58)*/
*++tmpPtr = -0.6432688236E-01;    /* ET(5,58)*/
*++tmpPtr = -0.1923017949E-01;    /* ET(6,58)*/
*++tmpPtr = -0.7201921195E-01;    /* ET(7,58)*/
*++tmpPtr = -0.6436207797E-02;    /* ET(8,58)*/
*++tmpPtr = -0.1436404139E-01;    /* ET(9,58)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  59: */
 /* */
*++tmpPtr =  0.5913625956E+00;    /* ET(1,59)*/
*++tmpPtr =  0.1123717502E+00;    /* ET(2,59)*/
*++tmpPtr =  0.2081519812E+00;    /* ET(3,59)*/
*++tmpPtr = -0.3322622925E-01;    /* ET(4,59)*/
*++tmpPtr = -0.6154664978E-01;    /* ET(5,59)*/
*++tmpPtr = -0.1169520244E-01;    /* ET(6,59)*/
*++tmpPtr = -0.8742743731E-01;    /* ET(7,59)*/
*++tmpPtr = -0.3156852676E-02;    /* ET(8,59)*/
*++tmpPtr = -0.1083181240E-01;    /* ET(9,59)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  60: */
 /* */
*++tmpPtr =  0.6097114086E+00;    /* ET(1,60)*/
*++tmpPtr =  0.1513359845E+00;    /* ET(2,60)*/
*++tmpPtr =  0.1200810149E+00;    /* ET(3,60)*/
*++tmpPtr = -0.4613563418E-01;    /* ET(4,60)*/
*++tmpPtr = -0.3660737723E-01;    /* ET(5,60)*/
*++tmpPtr = -0.9086288512E-02;    /* ET(6,60)*/
*++tmpPtr = -0.9293699265E-01;    /* ET(7,60)*/
*++tmpPtr = -0.5725644529E-02;    /* ET(8,60)*/
*++tmpPtr = -0.3604862606E-02;    /* ET(9,60)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  61: */
 /* */
*++tmpPtr =  0.5908721089E+00;    /* ET(1,61)*/
*++tmpPtr =  0.2220146209E+00;    /* ET(2,61)*/
*++tmpPtr =  0.2671248615E+00;    /* ET(3,61)*/
*++tmpPtr = -0.6559111923E-01;    /* ET(4,61)*/
*++tmpPtr = -0.7891830802E-01;    /* ET(5,61)*/
*++tmpPtr = -0.2965281159E-01;    /* ET(6,61)*/
*++tmpPtr = -0.8728245646E-01;    /* ET(7,61)*/
*++tmpPtr = -0.1232262328E-01;    /* ET(8,61)*/
*++tmpPtr = -0.1783892140E-01;    /* ET(9,61)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  62: */
 /* */
*++tmpPtr =  0.6746950746E+00;    /* ET(1,62)*/
*++tmpPtr =  0.2284755558E+00;    /* ET(2,62)*/
*++tmpPtr =  0.2523281872E+00;    /* ET(3,62)*/
*++tmpPtr = -0.7707566768E-01;    /* ET(4,62)*/
*++tmpPtr = -0.8512229472E-01;    /* ET(5,62)*/
*++tmpPtr = -0.2882541157E-01;    /* ET(6,62)*/
*++tmpPtr = -0.1138033569E+00;    /* ET(7,62)*/
*++tmpPtr = -0.1305026934E-01;    /* ET(8,62)*/
*++tmpPtr = -0.1591737941E-01;    /* ET(9,62)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  63: */
 /* */
*++tmpPtr =  0.6844686270E+00;    /* ET(1,63)*/
*++tmpPtr =  0.1495158821E+00;    /* ET(2,63)*/
*++tmpPtr =  0.2084181309E+00;    /* ET(3,63)*/
*++tmpPtr = -0.5116946250E-01;    /* ET(4,63)*/
*++tmpPtr = -0.7132782787E-01;    /* ET(5,63)*/
*++tmpPtr = -0.1558090933E-01;    /* ET(6,63)*/
*++tmpPtr = -0.1171243191E+00;    /* ET(7,63)*/
*++tmpPtr = -0.5588749889E-02;    /* ET(8,63)*/
*++tmpPtr = -0.1085952856E-01;    /* ET(9,63)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  64: */
 /* */
*++tmpPtr =  0.7498539686E+00;    /* ET(1,64)*/
*++tmpPtr =  0.2031289935E+00;    /* ET(2,64)*/
*++tmpPtr =  0.1401841342E+00;    /* ET(3,64)*/
*++tmpPtr = -0.7615854591E-01;    /* ET(4,64)*/
*++tmpPtr = -0.5255881324E-01;    /* ET(5,64)*/
*++tmpPtr = -0.1423773170E-01;    /* ET(6,64)*/
*++tmpPtr = -0.1405702531E+00;    /* ET(7,64)*/
*++tmpPtr = -0.1031534746E-01;    /* ET(8,64)*/
*++tmpPtr = -0.4912897479E-02;    /* ET(9,64)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  65: */
 /* */
*++tmpPtr =  0.1279055178E+00;    /* ET(1,65)*/
*++tmpPtr =  0.4388945997E+00;    /* ET(2,65)*/
*++tmpPtr =  0.2254860252E+00;    /* ET(3,65)*/
*++tmpPtr = -0.2806852199E-01;    /* ET(4,65)*/
*++tmpPtr = -0.1442045346E-01;    /* ET(5,65)*/
*++tmpPtr = -0.4948230088E-01;    /* ET(6,65)*/
*++tmpPtr = -0.4089955706E-02;    /* ET(7,65)*/
*++tmpPtr = -0.4815711826E-01;    /* ET(8,65)*/
*++tmpPtr = -0.1271098666E-01;    /* ET(9,65)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  66: */
 /* */
*++tmpPtr =  0.1764341593E+00;    /* ET(1,66)*/
*++tmpPtr =  0.4679493606E+00;    /* ET(2,66)*/
*++tmpPtr =  0.3378824890E+00;    /* ET(3,66)*/
*++tmpPtr = -0.4128112644E-01;    /* ET(4,66)*/
*++tmpPtr = -0.2980700508E-01;    /* ET(5,66)*/
*++tmpPtr = -0.7905595005E-01;    /* ET(6,66)*/
*++tmpPtr = -0.7782252505E-02;    /* ET(7,66)*/
*++tmpPtr = -0.5474415421E-01;    /* ET(8,66)*/
*++tmpPtr = -0.2854114398E-01;    /* ET(9,66)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  67: */
 /* */
*++tmpPtr =  0.2994391918E+00;    /* ET(1,67)*/
*++tmpPtr =  0.3769090772E+00;    /* ET(2,67)*/
*++tmpPtr =  0.2267388701E+00;    /* ET(3,67)*/
*++tmpPtr = -0.5643067509E-01;    /* ET(4,67)*/
*++tmpPtr = -0.3394725546E-01;    /* ET(5,67)*/
*++tmpPtr = -0.4272997007E-01;    /* ET(6,67)*/
*++tmpPtr = -0.2241595648E-01;    /* ET(7,67)*/
*++tmpPtr = -0.3551511094E-01;    /* ET(8,67)*/
*++tmpPtr = -0.1285262965E-01;    /* ET(9,67)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  68: */
 /* */
*++tmpPtr =  0.3704997599E+00;    /* ET(1,68)*/
*++tmpPtr =  0.3700670004E+00;    /* ET(2,68)*/
*++tmpPtr =  0.2853685021E+00;    /* ET(3,68)*/
*++tmpPtr = -0.6855487078E-01;    /* ET(4,68)*/
*++tmpPtr = -0.5286447704E-01;    /* ET(5,68)*/
*++tmpPtr = -0.5280273780E-01;    /* ET(6,68)*/
*++tmpPtr = -0.3431751952E-01;    /* ET(7,68)*/
*++tmpPtr = -0.3423739597E-01;    /* ET(8,68)*/
*++tmpPtr = -0.2035879716E-01;    /* ET(9,68)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  69: */
 /* */
*++tmpPtr =  0.2501392066E+00;    /* ET(1,69)*/
*++tmpPtr =  0.5540211797E+00;    /* ET(2,69)*/
*++tmpPtr =  0.2818284929E+00;    /* ET(3,69)*/
*++tmpPtr = -0.6929120421E-01;    /* ET(4,69)*/
*++tmpPtr = -0.3524817526E-01;    /* ET(5,69)*/
*++tmpPtr = -0.7806948572E-01;    /* ET(6,69)*/
*++tmpPtr = -0.1564240456E-01;    /* ET(7,69)*/
*++tmpPtr = -0.7673487067E-01;    /* ET(8,69)*/
*++tmpPtr = -0.1985682547E-01;    /* ET(9,69)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  70: */
 /* */
*++tmpPtr =  0.3003772497E+00;    /* ET(1,70)*/
*++tmpPtr =  0.6232951283E+00;    /* ET(2,70)*/
*++tmpPtr =  0.3412563205E+00;    /* ET(3,70)*/
*++tmpPtr = -0.9361184388E-01;    /* ET(4,70)*/
*++tmpPtr = -0.5125281587E-01;    /* ET(5,70)*/
*++tmpPtr = -0.1063517034E+00;    /* ET(6,70)*/
*++tmpPtr = -0.2255662344E-01;    /* ET(7,70)*/
*++tmpPtr = -0.9712421149E-01;    /* ET(8,70)*/
*++tmpPtr = -0.2911397070E-01;    /* ET(9,70)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  71: */
 /* */
*++tmpPtr =  0.1402098238E+00;    /* ET(1,71)*/
*++tmpPtr =  0.6274347305E+00;    /* ET(2,71)*/
*++tmpPtr =  0.5752885342E+00;    /* ET(3,71)*/
*++tmpPtr = -0.4398625717E-01;    /* ET(4,71)*/
*++tmpPtr = -0.4033055156E-01;    /* ET(5,71)*/
*++tmpPtr = -0.1804779917E+00;    /* ET(6,71)*/
*++tmpPtr = -0.4914698657E-02;    /* ET(7,71)*/
*++tmpPtr = -0.9841858596E-01;    /* ET(8,71)*/
*++tmpPtr = -0.8273921907E-01;    /* ET(9,71)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  72: */
 /* */
*++tmpPtr =  0.2425548881E+00;    /* ET(1,72)*/
*++tmpPtr =  0.6668438911E+00;    /* ET(2,72)*/
*++tmpPtr =  0.8577076197E+00;    /* ET(3,72)*/
*++tmpPtr = -0.8087312430E-01;    /* ET(4,72)*/
*++tmpPtr = -0.1040205881E+00;    /* ET(5,72)*/
*++tmpPtr = -0.2859785557E+00;    /* ET(6,72)*/
*++tmpPtr = -0.1470821910E-01;    /* ET(7,72)*/
*++tmpPtr = -0.1111702025E+00;    /* ET(8,72)*/
*++tmpPtr = -0.1839156002E+00;    /* ET(9,72)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  73: */
 /* */
*++tmpPtr =  0.2635775506E+00;    /* ET(1,73)*/
*++tmpPtr =  0.4350323677E+00;    /* ET(2,73)*/
*++tmpPtr =  0.4462251961E+00;    /* ET(3,73)*/
*++tmpPtr = -0.5733238161E-01;    /* ET(4,73)*/
*++tmpPtr = -0.5880747363E-01;    /* ET(5,73)*/
*++tmpPtr = -0.9706119448E-01;    /* ET(6,73)*/
*++tmpPtr = -0.1736828312E-01;    /* ET(7,73)*/
*++tmpPtr = -0.4731328785E-01;    /* ET(8,73)*/
*++tmpPtr = -0.4977922887E-01;    /* ET(9,73)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  74: */
 /* */
*++tmpPtr =  0.3390684724E+00;    /* ET(1,74)*/
*++tmpPtr =  0.4535474181E+00;    /* ET(2,74)*/
*++tmpPtr =  0.5314882398E+00;    /* ET(3,74)*/
*++tmpPtr = -0.7689180970E-01;    /* ET(4,74)*/
*++tmpPtr = -0.9010544419E-01;    /* ET(5,74)*/
*++tmpPtr = -0.1205275506E+00;    /* ET(6,74)*/
*++tmpPtr = -0.2874185517E-01;    /* ET(7,74)*/
*++tmpPtr = -0.5142631382E-01;    /* ET(8,74)*/
*++tmpPtr = -0.7061993331E-01;    /* ET(9,74)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  75: */
 /* */
*++tmpPtr =  0.3931885958E+00;    /* ET(1,75)*/
*++tmpPtr =  0.2931576967E+00;    /* ET(2,75)*/
*++tmpPtr =  0.6647785306E+00;    /* ET(3,75)*/
*++tmpPtr = -0.5763313547E-01;    /* ET(4,75)*/
*++tmpPtr = -0.1306916773E+00;    /* ET(5,75)*/
*++tmpPtr = -0.9744246304E-01;    /* ET(6,75)*/
*++tmpPtr = -0.3864932060E-01;    /* ET(7,75)*/
*++tmpPtr = -0.2148535848E-01;    /* ET(8,75)*/
*++tmpPtr = -0.1104826182E+00;    /* ET(9,75)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  76: */
 /* */
*++tmpPtr =  0.5275677443E+00;    /* ET(1,76)*/
*++tmpPtr =  0.2324023843E+00;    /* ET(2,76)*/
*++tmpPtr =  0.7639622092E+00;    /* ET(3,76)*/
*++tmpPtr = -0.6130399927E-01;    /* ET(4,76)*/
*++tmpPtr = -0.2015209049E+00;    /* ET(5,76)*/
*++tmpPtr = -0.8877331764E-01;    /* ET(6,76)*/
*++tmpPtr = -0.6958192587E-01;    /* ET(7,76)*/
*++tmpPtr = -0.1350271702E-01;    /* ET(8,76)*/
*++tmpPtr = -0.1459095627E+00;    /* ET(9,76)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  77: */
 /* */
*++tmpPtr =  0.3986386061E+00;    /* ET(1,77)*/
*++tmpPtr =  0.4638858736E+00;    /* ET(2,77)*/
*++tmpPtr =  0.2884156108E+00;    /* ET(3,77)*/
*++tmpPtr = -0.9246141464E-01;    /* ET(4,77)*/
*++tmpPtr = -0.5748679861E-01;    /* ET(5,77)*/
*++tmpPtr = -0.6689595431E-01;    /* ET(6,77)*/
*++tmpPtr = -0.3972818702E-01;    /* ET(7,77)*/
*++tmpPtr = -0.5379752815E-01;    /* ET(8,77)*/
*++tmpPtr = -0.2079588920E-01;    /* ET(9,77)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  78: */
 /* */
*++tmpPtr =  0.4666251838E+00;    /* ET(1,78)*/
*++tmpPtr =  0.4136271179E+00;    /* ET(2,78)*/
*++tmpPtr =  0.3964774609E+00;    /* ET(3,78)*/
*++tmpPtr = -0.9650441259E-01;    /* ET(4,78)*/
*++tmpPtr = -0.9250318259E-01;    /* ET(5,78)*/
*++tmpPtr = -0.8199691027E-01;    /* ET(6,78)*/
*++tmpPtr = -0.5443476513E-01;    /* ET(7,78)*/
*++tmpPtr = -0.4277184606E-01;    /* ET(8,78)*/
*++tmpPtr = -0.3929859400E-01;    /* ET(9,78)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  79: */
 /* */
*++tmpPtr =  0.5350810885E+00;    /* ET(1,79)*/
*++tmpPtr =  0.4313596189E+00;    /* ET(2,79)*/
*++tmpPtr =  0.2718358040E+00;    /* ET(3,79)*/
*++tmpPtr = -0.1154061854E+00;    /* ET(4,79)*/
*++tmpPtr = -0.7272709906E-01;    /* ET(5,79)*/
*++tmpPtr = -0.5862949416E-01;    /* ET(6,79)*/
*++tmpPtr = -0.7157794386E-01;    /* ET(7,79)*/
*++tmpPtr = -0.4651777819E-01;    /* ET(8,79)*/
*++tmpPtr = -0.1847367734E-01;    /* ET(9,79)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  80: */
 /* */
*++tmpPtr =  0.6371671557E+00;    /* ET(1,80)*/
*++tmpPtr =  0.4336667359E+00;    /* ET(2,80)*/
*++tmpPtr =  0.2950619459E+00;    /* ET(3,80)*/
*++tmpPtr = -0.1381590962E+00;    /* ET(4,80)*/
*++tmpPtr = -0.9400188923E-01;    /* ET(5,80)*/
*++tmpPtr = -0.6397927552E-01;    /* ET(6,80)*/
*++tmpPtr = -0.1014954969E+00;    /* ET(7,80)*/
*++tmpPtr = -0.4701671004E-01;    /* ET(8,80)*/
*++tmpPtr = -0.2176538669E-01;    /* ET(9,80)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  81: */
 /* */
*++tmpPtr =  0.4213872552E+00;    /* ET(1,81)*/
*++tmpPtr =  0.5714991093E+00;    /* ET(2,81)*/
*++tmpPtr =  0.4178683758E+00;    /* ET(3,81)*/
*++tmpPtr = -0.1204112172E+00;    /* ET(4,81)*/
*++tmpPtr = -0.8804219961E-01;    /* ET(5,81)*/
*++tmpPtr = -0.1194057018E+00;    /* ET(6,81)*/
*++tmpPtr = -0.4439180344E-01;    /* ET(7,81)*/
*++tmpPtr = -0.8165280521E-01;    /* ET(8,81)*/
*++tmpPtr = -0.4365349561E-01;    /* ET(9,81)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  82: */
 /* */
*++tmpPtr =  0.4997817576E+00;    /* ET(1,82)*/
*++tmpPtr =  0.4871748984E+00;    /* ET(2,82)*/
*++tmpPtr =  0.5042631030E+00;    /* ET(3,82)*/
*++tmpPtr = -0.1217405573E+00;    /* ET(4,82)*/
*++tmpPtr = -0.1260107309E+00;    /* ET(5,82)*/
*++tmpPtr = -0.1228321493E+00;    /* ET(6,82)*/
*++tmpPtr = -0.6244545057E-01;    /* ET(7,82)*/
*++tmpPtr = -0.5933484435E-01;    /* ET(8,82)*/
*++tmpPtr = -0.6357031316E-01;    /* ET(9,82)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  83: */
 /* */
*++tmpPtr =  0.4555468559E+00;    /* ET(1,83)*/
*++tmpPtr =  0.6004845500E+00;    /* ET(2,83)*/
*++tmpPtr =  0.6253300905E+00;    /* ET(3,83)*/
*++tmpPtr = -0.1367744207E+00;    /* ET(4,83)*/
*++tmpPtr = -0.1424335688E+00;    /* ET(5,83)*/
*++tmpPtr = -0.1877505332E+00;    /* ET(6,83)*/
*++tmpPtr = -0.5188073218E-01;    /* ET(7,83)*/
*++tmpPtr = -0.9014543146E-01;    /* ET(8,83)*/
*++tmpPtr = -0.9775943309E-01;    /* ET(9,83)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  84: */
 /* */
*++tmpPtr =  0.6128560305E+00;    /* ET(1,84)*/
*++tmpPtr =  0.5508396626E+00;    /* ET(2,84)*/
*++tmpPtr =  0.5597388744E+00;    /* ET(3,84)*/
*++tmpPtr = -0.1687926948E+00;    /* ET(4,84)*/
*++tmpPtr = -0.1715196818E+00;    /* ET(5,84)*/
*++tmpPtr = -0.1541631818E+00;    /* ET(6,84)*/
*++tmpPtr = -0.9389812499E-01;    /* ET(7,84)*/
*++tmpPtr = -0.7585608214E-01;    /* ET(8,84)*/
*++tmpPtr = -0.7832690328E-01;    /* ET(9,84)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  85: */
 /* */
*++tmpPtr =  0.4279587567E+00;    /* ET(1,85)*/
*++tmpPtr =  0.4398245513E+00;    /* ET(2,85)*/
*++tmpPtr =  0.7074258327E+00;    /* ET(3,85)*/
*++tmpPtr = -0.9411338717E-01;    /* ET(4,85)*/
*++tmpPtr = -0.1513745338E+00;    /* ET(5,85)*/
*++tmpPtr = -0.1555716246E+00;    /* ET(6,85)*/
*++tmpPtr = -0.4578717425E-01;    /* ET(7,85)*/
*++tmpPtr = -0.4836140946E-01;    /* ET(8,85)*/
*++tmpPtr = -0.1251128167E+00;    /* ET(9,85)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  86: */
 /* */
*++tmpPtr =  0.5034425855E+00;    /* ET(1,86)*/
*++tmpPtr =  0.4188446105E+00;    /* ET(2,86)*/
*++tmpPtr =  0.8750000596E+00;    /* ET(3,86)*/
*++tmpPtr = -0.1054321080E+00;    /* ET(4,86)*/
*++tmpPtr = -0.2202561498E+00;    /* ET(5,86)*/
*++tmpPtr = -0.1832445264E+00;    /* ET(6,86)*/
*++tmpPtr = -0.6336360425E-01;    /* ET(7,86)*/
*++tmpPtr = -0.4385770485E-01;    /* ET(8,86)*/
*++tmpPtr = -0.1914062798E+00;    /* ET(9,86)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  87: */
 /* */
*++tmpPtr =  0.6318414211E+00;    /* ET(1,87)*/
*++tmpPtr =  0.4656572342E+00;    /* ET(2,87)*/
*++tmpPtr =  0.8587746024E+00;    /* ET(3,87)*/
*++tmpPtr = -0.1471107602E+00;    /* ET(4,87)*/
*++tmpPtr = -0.2713046670E+00;    /* ET(5,87)*/
*++tmpPtr = -0.1999472976E+00;    /* ET(6,87)*/
*++tmpPtr = -0.9980589896E-01;    /* ET(7,87)*/
*++tmpPtr = -0.5420916155E-01;    /* ET(8,87)*/
*++tmpPtr = -0.1843734533E+00;    /* ET(9,87)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  88: */
 /* */
*++tmpPtr =  0.7873216271E+00;    /* ET(1,88)*/
*++tmpPtr =  0.4170691967E+00;    /* ET(2,88)*/
*++tmpPtr =  0.8374727964E+00;    /* ET(3,88)*/
*++tmpPtr = -0.1641838104E+00;    /* ET(4,88)*/
*++tmpPtr = -0.3296802342E+00;    /* ET(5,88)*/
*++tmpPtr = -0.1746420562E+00;    /* ET(6,88)*/
*++tmpPtr = -0.1549688429E+00;    /* ET(7,88)*/
*++tmpPtr = -0.4348667711E-01;    /* ET(8,88)*/
*++tmpPtr = -0.1753401756E+00;    /* ET(9,88)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  89: */
 /* */
*++tmpPtr =  0.5328483582E+00;    /* ET(1,89)*/
*++tmpPtr =  0.5697864294E+00;    /* ET(2,89)*/
*++tmpPtr =  0.3083459735E+00;    /* ET(3,89)*/
*++tmpPtr = -0.1518048942E+00;    /* ET(4,89)*/
*++tmpPtr = -0.8215082437E-01;    /* ET(5,89)*/
*++tmpPtr = -0.8784567565E-01;    /* ET(6,89)*/
*++tmpPtr = -0.7098184526E-01;    /* ET(7,89)*/
*++tmpPtr = -0.8116415143E-01;    /* ET(8,89)*/
*++tmpPtr = -0.2376930974E-01;    /* ET(9,89)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  90: */
 /* */
*++tmpPtr =  0.6417773366E+00;    /* ET(1,90)*/
*++tmpPtr =  0.5875701308E+00;    /* ET(2,90)*/
*++tmpPtr =  0.3667091727E+00;    /* ET(3,90)*/
*++tmpPtr = -0.1885446012E+00;    /* ET(4,90)*/
*++tmpPtr = -0.1176728159E+00;    /* ET(5,90)*/
*++tmpPtr = -0.1077336743E+00;    /* ET(6,90)*/
*++tmpPtr = -0.1029695347E+00;    /* ET(7,90)*/
*++tmpPtr = -0.8630966395E-01;    /* ET(8,90)*/
*++tmpPtr = -0.3361890465E-01;    /* ET(9,90)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  91: */
 /* */
*++tmpPtr =  0.7228266597E+00;    /* ET(1,91)*/
*++tmpPtr =  0.4422772825E+00;    /* ET(2,91)*/
*++tmpPtr =  0.3886556327E+00;    /* ET(3,91)*/
*++tmpPtr = -0.1598449051E+00;    /* ET(4,91)*/
*++tmpPtr = -0.1404653341E+00;    /* ET(5,91)*/
*++tmpPtr = -0.8594677597E-01;    /* ET(6,91)*/
*++tmpPtr = -0.1306196004E+00;    /* ET(7,91)*/
*++tmpPtr = -0.4890229553E-01;    /* ET(8,91)*/
*++tmpPtr = -0.3776329756E-01;    /* ET(9,91)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  92: */
 /* */
*++tmpPtr =  0.8217436075E+00;    /* ET(1,92)*/
*++tmpPtr =  0.4964416623E+00;    /* ET(2,92)*/
*++tmpPtr =  0.2776305377E+00;    /* ET(3,92)*/
*++tmpPtr = -0.2039738744E+00;    /* ET(4,92)*/
*++tmpPtr = -0.1140705645E+00;    /* ET(5,92)*/
*++tmpPtr = -0.6891369075E-01;    /* ET(6,92)*/
*++tmpPtr = -0.1688156426E+00;    /* ET(7,92)*/
*++tmpPtr = -0.6161358207E-01;    /* ET(8,92)*/
*++tmpPtr = -0.1926968060E-01;    /* ET(9,92)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  93: */
 /* */
*++tmpPtr =  0.7301130891E+00;    /* ET(1,93)*/
*++tmpPtr =  0.5760177374E+00;    /* ET(2,93)*/
*++tmpPtr =  0.4804499149E+00;    /* ET(3,93)*/
*++tmpPtr = -0.2102790326E+00;    /* ET(4,93)*/
*++tmpPtr = -0.1753913760E+00;    /* ET(5,93)*/
*++tmpPtr = -0.1383738369E+00;    /* ET(6,93)*/
*++tmpPtr = -0.1332662851E+00;    /* ET(7,93)*/
*++tmpPtr = -0.8294910192E-01;    /* ET(8,93)*/
*++tmpPtr = -0.5770802870E-01;    /* ET(9,93)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  94: */
 /* */
*++tmpPtr =  0.8579565883E+00;    /* ET(1,94)*/
*++tmpPtr =  0.5996007919E+00;    /* ET(2,94)*/
*++tmpPtr =  0.5137856603E+00;    /* ET(3,94)*/
*++tmpPtr = -0.2572157383E+00;    /* ET(4,94)*/
*++tmpPtr = -0.2204028964E+00;    /* ET(5,94)*/
*++tmpPtr = -0.1540331393E+00;    /* ET(6,94)*/
*++tmpPtr = -0.1840223819E+00;    /* ET(7,94)*/
*++tmpPtr = -0.8988028020E-01;    /* ET(8,94)*/
*++tmpPtr = -0.6599391997E-01;    /* ET(9,94)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  95: */
 /* */
*++tmpPtr =  0.8439285159E+00;    /* ET(1,95)*/
*++tmpPtr =  0.7065181732E+00;    /* ET(2,95)*/
*++tmpPtr =  0.3803812563E+00;    /* ET(3,95)*/
*++tmpPtr = -0.2981254160E+00;    /* ET(4,95)*/
*++tmpPtr = -0.1605072916E+00;    /* ET(5,95)*/
*++tmpPtr = -0.1343731433E+00;    /* ET(6,95)*/
*++tmpPtr = -0.1780538261E+00;    /* ET(7,95)*/
*++tmpPtr = -0.1247919872E+00;    /* ET(8,95)*/
*++tmpPtr = -0.3617247567E-01;    /* ET(9,95)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  96: */
 /* */
*++tmpPtr =  0.1043463826E+01;    /* ET(1,96)*/
*++tmpPtr =  0.6582704782E+00;    /* ET(2,96)*/
*++tmpPtr =  0.3782508969E+00;    /* ET(3,96)*/
*++tmpPtr = -0.3434407115E+00;    /* ET(4,96)*/
*++tmpPtr = -0.1973455548E+00;    /* ET(5,96)*/
*++tmpPtr = -0.1244957000E+00;    /* ET(6,96)*/
*++tmpPtr = -0.2722041607E+00;    /* ET(7,96)*/
*++tmpPtr = -0.1083300114E+00;    /* ET(8,96)*/
*++tmpPtr = -0.3576843441E-01;    /* ET(9,96)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  97: */
 /* */
*++tmpPtr =  0.4639641345E+00;    /* ET(1,97)*/
*++tmpPtr =  0.3171636760E+00;    /* ET(2,97)*/
*++tmpPtr =  0.4585282207E+00;    /* ET(3,97)*/
*++tmpPtr = -0.7357628644E-01;    /* ET(4,97)*/
*++tmpPtr = -0.1063703150E+00;    /* ET(5,97)*/
*++tmpPtr = -0.7271424681E-01;    /* ET(6,97)*/
*++tmpPtr = -0.5381567776E-01;    /* ET(7,97)*/
*++tmpPtr = -0.2514819987E-01;    /* ET(8,97)*/
*++tmpPtr = -0.5256203189E-01;    /* ET(9,97)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  98: */
 /* */
*++tmpPtr =  0.5764331222E+00;    /* ET(1,98)*/
*++tmpPtr =  0.3435526490E+00;    /* ET(2,98)*/
*++tmpPtr =  0.4391970634E+00;    /* ET(3,98)*/
*++tmpPtr = -0.9901756048E-01;    /* ET(4,98)*/
*++tmpPtr = -0.1265838593E+00;    /* ET(5,98)*/
*++tmpPtr = -0.7544365525E-01;    /* ET(6,98)*/
*++tmpPtr = -0.8306878060E-01;    /* ET(7,98)*/
*++tmpPtr = -0.2950710803E-01;    /* ET(8,98)*/
*++tmpPtr = -0.4822351411E-01;    /* ET(9,98)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID #  99: */
 /* */
*++tmpPtr =  0.6037842631E+00;    /* ET(1,99)*/
*++tmpPtr =  0.2576600015E+00;    /* ET(2,99)*/
*++tmpPtr =  0.5663941503E+00;    /* ET(3,99)*/
*++tmpPtr = -0.7778552175E-01;    /* ET(4,99)*/
*++tmpPtr = -0.1709899306E+00;    /* ET(5,99)*/
*++tmpPtr = -0.7296855003E-01;    /* ET(6,99)*/
*++tmpPtr = -0.9113886207E-01;    /* ET(7,99)*/
*++tmpPtr = -0.1659716852E-01;    /* ET(8,99)*/
*++tmpPtr = -0.8020057529E-01;    /* ET(9,99)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 100: */
 /* */
*++tmpPtr =  0.7082279325E+00;    /* ET(1,100)*/
*++tmpPtr =  0.3161120415E+00;    /* ET(2,100)*/
*++tmpPtr =  0.4750343263E+00;    /* ET(3,100)*/
*++tmpPtr = -0.1119396910E+00;    /* ET(4,100)*/
*++tmpPtr = -0.1682162732E+00;    /* ET(5,100)*/
*++tmpPtr = -0.7508204132E-01;    /* ET(6,100)*/
*++tmpPtr = -0.1253966987E+00;    /* ET(7,100)*/
*++tmpPtr = -0.2498170733E-01;    /* ET(8,100)*/
*++tmpPtr = -0.5641439930E-01;    /* ET(9,100)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 101: */
 /* */
*++tmpPtr =  0.6265090108E+00;    /* ET(1,101)*/
*++tmpPtr =  0.3159430027E+00;    /* ET(2,101)*/
*++tmpPtr =  0.3452484608E+00;    /* ET(3,101)*/
*++tmpPtr = -0.9897056967E-01;    /* ET(4,101)*/
*++tmpPtr = -0.1081506312E+00;    /* ET(5,101)*/
*++tmpPtr = -0.5453941599E-01;    /* ET(6,101)*/
*++tmpPtr = -0.9812837839E-01;    /* ET(7,101)*/
*++tmpPtr = -0.2495499514E-01;    /* ET(8,101)*/
*++tmpPtr = -0.2979912609E-01;    /* ET(9,101)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 102: */
 /* */
*++tmpPtr =  0.7133237720E+00;    /* ET(1,102)*/
*++tmpPtr =  0.3240157962E+00;    /* ET(2,102)*/
*++tmpPtr =  0.3149312735E+00;    /* ET(3,102)*/
*++tmpPtr = -0.1155640855E+00;    /* ET(4,102)*/
*++tmpPtr = -0.1123239920E+00;    /* ET(5,102)*/
*++tmpPtr = -0.5102135241E-01;    /* ET(6,102)*/
*++tmpPtr = -0.1272077113E+00;    /* ET(7,102)*/
*++tmpPtr = -0.2624655701E-01;    /* ET(8,102)*/
*++tmpPtr = -0.2479542792E-01;    /* ET(9,102)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 103: */
 /* */
*++tmpPtr =  0.7890176177E+00;    /* ET(1,103)*/
*++tmpPtr =  0.2793453038E+00;    /* ET(2,103)*/
*++tmpPtr =  0.2823780477E+00;    /* ET(3,103)*/
*++tmpPtr = -0.1102041900E+00;    /* ET(4,103)*/
*++tmpPtr = -0.1114006266E+00;    /* ET(5,103)*/
*++tmpPtr = -0.3944049031E-01;    /* ET(6,103)*/
*++tmpPtr = -0.1556372046E+00;    /* ET(7,103)*/
*++tmpPtr = -0.1950844936E-01;    /* ET(8,103)*/
*++tmpPtr = -0.1993434131E-01;    /* ET(9,103)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 104: */
 /* */
*++tmpPtr =  0.8588279486E+00;    /* ET(1,104)*/
*++tmpPtr =  0.2983707488E+00;    /* ET(2,104)*/
*++tmpPtr =  0.2004490644E+00;    /* ET(3,104)*/
*++tmpPtr = -0.1281245798E+00;    /* ET(4,104)*/
*++tmpPtr = -0.8607563376E-01;    /* ET(5,104)*/
*++tmpPtr = -0.2990406752E-01;    /* ET(6,104)*/
*++tmpPtr = -0.1843963712E+00;    /* ET(7,104)*/
*++tmpPtr = -0.2225627564E-01;    /* ET(8,104)*/
*++tmpPtr = -0.1004495658E-01;    /* ET(9,104)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 105: */
 /* */
*++tmpPtr =  0.6523181200E+00;    /* ET(1,105)*/
*++tmpPtr =  0.1780289710E+00;    /* ET(2,105)*/
*++tmpPtr =  0.4111101627E+00;    /* ET(3,105)*/
*++tmpPtr = -0.5806576461E-01;    /* ET(4,105)*/
*++tmpPtr = -0.1340873092E+00;    /* ET(5,105)*/
*++tmpPtr = -0.3659475967E-01;    /* ET(6,105)*/
*++tmpPtr = -0.1063797325E+00;    /* ET(7,105)*/
*++tmpPtr = -0.7923578843E-02;    /* ET(8,105)*/
*++tmpPtr = -0.4225289077E-01;    /* ET(9,105)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 106: */
 /* */
*++tmpPtr =  0.7645785809E+00;    /* ET(1,106)*/
*++tmpPtr =  0.1430492848E+00;    /* ET(2,106)*/
*++tmpPtr =  0.3669104576E+00;    /* ET(3,106)*/
*++tmpPtr = -0.5468621477E-01;    /* ET(4,106)*/
*++tmpPtr = -0.1402659565E+00;    /* ET(5,106)*/
*++tmpPtr = -0.2624314092E-01;    /* ET(6,106)*/
*++tmpPtr = -0.1461451054E+00;    /* ET(7,106)*/
*++tmpPtr = -0.5115774926E-02;    /* ET(8,106)*/
*++tmpPtr = -0.3365582228E-01;    /* ET(9,106)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 107: */
 /* */
*++tmpPtr =  0.7833608985E+00;    /* ET(1,107)*/
*++tmpPtr =  0.1790251285E+00;    /* ET(2,107)*/
*++tmpPtr =  0.2384854108E+00;    /* ET(3,107)*/
*++tmpPtr = -0.7012064010E-01;    /* ET(4,107)*/
*++tmpPtr = -0.9341006726E-01;    /* ET(5,107)*/
*++tmpPtr = -0.2134743892E-01;    /* ET(6,107)*/
*++tmpPtr = -0.1534135789E+00;    /* ET(7,107)*/
*++tmpPtr = -0.8012498729E-02;    /* ET(8,107)*/
*++tmpPtr = -0.1421882305E-01;    /* ET(9,107)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 108: */
 /* */
*++tmpPtr =  0.8306412101E+00;    /* ET(1,108)*/
*++tmpPtr =  0.1064326838E+00;    /* ET(2,108)*/
*++tmpPtr =  0.1728887111E+00;    /* ET(3,108)*/
*++tmpPtr = -0.4420368373E-01;    /* ET(4,108)*/
*++tmpPtr = -0.7180424035E-01;    /* ET(5,108)*/
*++tmpPtr = -0.9200504981E-02;    /* ET(6,108)*/
*++tmpPtr = -0.1724911928E+00;    /* ET(7,108)*/
*++tmpPtr = -0.2831978956E-02;    /* ET(8,108)*/
*++tmpPtr = -0.7472626865E-02;    /* ET(9,108)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 109: */
 /* */
*++tmpPtr =  0.8498703837E+00;    /* ET(1,109)*/
*++tmpPtr =  0.1816121489E+00;    /* ET(2,109)*/
*++tmpPtr =  0.3441391885E+00;    /* ET(3,109)*/
*++tmpPtr = -0.7717338949E-01;    /* ET(4,109)*/
*++tmpPtr = -0.1462368518E+00;    /* ET(5,109)*/
*++tmpPtr = -0.3124992922E-01;    /* ET(6,109)*/
*++tmpPtr = -0.1805699170E+00;    /* ET(7,109)*/
*++tmpPtr = -0.8245742880E-02;    /* ET(8,109)*/
*++tmpPtr = -0.2960794605E-01;    /* ET(9,109)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 110: */
 /* */
*++tmpPtr =  0.9197843671E+00;    /* ET(1,110)*/
*++tmpPtr =  0.2197215259E+00;    /* ET(2,110)*/
*++tmpPtr =  0.2858580351E+00;    /* ET(3,110)*/
*++tmpPtr = -0.1010482013E+00;    /* ET(4,110)*/
*++tmpPtr = -0.1314638704E+00;    /* ET(5,110)*/
*++tmpPtr = -0.3140458092E-01;    /* ET(6,110)*/
*++tmpPtr = -0.2115008086E+00;    /* ET(7,110)*/
*++tmpPtr = -0.1206938643E-01;    /* ET(8,110)*/
*++tmpPtr = -0.2042870596E-01;    /* ET(9,110)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 111: */
 /* */
*++tmpPtr =  0.9212773442E+00;    /* ET(1,111)*/
*++tmpPtr =  0.1544044763E+00;    /* ET(2,111)*/
*++tmpPtr =  0.2138553560E+00;    /* ET(3,111)*/
*++tmpPtr = -0.7112467289E-01;    /* ET(4,111)*/
*++tmpPtr = -0.9851004928E-01;    /* ET(5,111)*/
*++tmpPtr = -0.1651011221E-01;    /* ET(6,111)*/
*++tmpPtr = -0.2121879905E+00;    /* ET(7,111)*/
*++tmpPtr = -0.5960185081E-02;    /* ET(8,111)*/
*++tmpPtr = -0.1143352780E-01;    /* ET(9,111)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 112: */
 /* */
*++tmpPtr =  0.9991567731E+00;    /* ET(1,112)*/
*++tmpPtr =  0.1788201332E+00;    /* ET(2,112)*/
*++tmpPtr =  0.1557907015E+00;    /* ET(3,112)*/
*++tmpPtr = -0.8933467418E-01;    /* ET(4,112)*/
*++tmpPtr = -0.7782965899E-01;    /* ET(5,112)*/
*++tmpPtr = -0.1392925717E-01;    /* ET(6,112)*/
*++tmpPtr = -0.2495785654E+00;    /* ET(7,112)*/
*++tmpPtr = -0.7994160056E-02;    /* ET(8,112)*/
*++tmpPtr = -0.6067685317E-02;    /* ET(9,112)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 113: */
 /* */
*++tmpPtr =  0.7607141137E+00;    /* ET(1,113)*/
*++tmpPtr =  0.1840053201E+00;    /* ET(2,113)*/
*++tmpPtr =  0.6403049827E+00;    /* ET(3,113)*/
*++tmpPtr = -0.6998772174E-01;    /* ET(4,113)*/
*++tmpPtr = -0.2435445189E+00;    /* ET(5,113)*/
*++tmpPtr = -0.5890976265E-01;    /* ET(6,113)*/
*++tmpPtr = -0.1446714848E+00;    /* ET(7,113)*/
*++tmpPtr = -0.8464490063E-02;    /* ET(8,113)*/
*++tmpPtr = -0.1024976149E+00;    /* ET(9,113)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 114: */
 /* */
*++tmpPtr =  0.8236495852E+00;    /* ET(1,114)*/
*++tmpPtr =  0.2309473306E+00;    /* ET(2,114)*/
*++tmpPtr =  0.4618392289E+00;    /* ET(3,114)*/
*++tmpPtr = -0.9510984272E-01;    /* ET(4,114)*/
*++tmpPtr = -0.1901968420E+00;    /* ET(5,114)*/
*++tmpPtr = -0.5333026871E-01;    /* ET(6,114)*/
*++tmpPtr = -0.1695996672E+00;    /* ET(7,114)*/
*++tmpPtr = -0.1333416812E-01;    /* ET(8,114)*/
*++tmpPtr = -0.5332386494E-01;    /* ET(9,114)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 115: */
 /* */
*++tmpPtr =  0.9205271602E+00;    /* ET(1,115)*/
*++tmpPtr =  0.2367242873E+00;    /* ET(2,115)*/
*++tmpPtr =  0.5592310429E+00;    /* ET(3,115)*/
*++tmpPtr = -0.1089555696E+00;    /* ET(4,115)*/
*++tmpPtr = -0.2573936880E+00;    /* ET(5,115)*/
*++tmpPtr = -0.6619178504E-01;    /* ET(6,115)*/
*++tmpPtr = -0.2118425667E+00;    /* ET(7,115)*/
*++tmpPtr = -0.1400959771E-01;    /* ET(8,115)*/
*++tmpPtr = -0.7818484306E-01;    /* ET(9,115)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 116: */
 /* */
*++tmpPtr =  0.9849490523E+00;    /* ET(1,116)*/
*++tmpPtr =  0.2734104395E+00;    /* ET(2,116)*/
*++tmpPtr =  0.4171831310E+00;    /* ET(3,116)*/
*++tmpPtr = -0.1346476674E+00;    /* ET(4,116)*/
*++tmpPtr = -0.2054520696E+00;    /* ET(5,116)*/
*++tmpPtr = -0.5703110993E-01;    /* ET(6,116)*/
*++tmpPtr = -0.2425311506E+00;    /* ET(7,116)*/
*++tmpPtr = -0.1868831739E-01;    /* ET(8,116)*/
*++tmpPtr = -0.4351044074E-01;    /* ET(9,116)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 117: */
 /* */
*++tmpPtr =  0.8485632539E+00;    /* ET(1,117)*/
*++tmpPtr =  0.3560683131E+00;    /* ET(2,117)*/
*++tmpPtr =  0.3844498694E+00;    /* ET(3,117)*/
*++tmpPtr = -0.1510732472E+00;    /* ET(4,117)*/
*++tmpPtr = -0.1631150246E+00;    /* ET(5,117)*/
*++tmpPtr = -0.6844521314E-01;    /* ET(6,117)*/
*++tmpPtr = -0.1800149083E+00;    /* ET(7,117)*/
*++tmpPtr = -0.3169615939E-01;    /* ET(8,117)*/
*++tmpPtr = -0.3695042804E-01;    /* ET(9,117)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 118: */
 /* */
*++tmpPtr =  0.9476958513E+00;    /* ET(1,118)*/
*++tmpPtr =  0.3350292742E+00;    /* ET(2,118)*/
*++tmpPtr =  0.2673843503E+00;    /* ET(3,118)*/
*++tmpPtr = -0.1587529182E+00;    /* ET(4,118)*/
*++tmpPtr = -0.1266995221E+00;    /* ET(5,118)*/
*++tmpPtr = -0.4479079321E-01;    /* ET(6,118)*/
*++tmpPtr = -0.2245318443E+00;    /* ET(7,118)*/
*++tmpPtr = -0.2806115337E-01;    /* ET(8,118)*/
*++tmpPtr = -0.1787359826E-01;    /* ET(9,118)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 119: */
 /* */
*++tmpPtr =  0.1046132922E+01;    /* ET(1,119)*/
*++tmpPtr =  0.3101744354E+00;    /* ET(2,119)*/
*++tmpPtr =  0.3292212486E+00;    /* ET(3,119)*/
*++tmpPtr = -0.1622418463E+00;    /* ET(4,119)*/
*++tmpPtr = -0.1722045839E+00;    /* ET(5,119)*/
*++tmpPtr = -0.5105800554E-01;    /* ET(6,119)*/
*++tmpPtr = -0.2735984921E+00;    /* ET(7,119)*/
*++tmpPtr = -0.2405204624E-01;    /* ET(8,119)*/
*++tmpPtr = -0.2709665522E-01;    /* ET(9,119)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 120: */
 /* */
*++tmpPtr =  0.1094669342E+01;    /* ET(1,120)*/
*++tmpPtr =  0.3106435835E+00;    /* ET(2,120)*/
*++tmpPtr =  0.2023477405E+00;    /* ET(3,120)*/
*++tmpPtr = -0.1700260043E+00;    /* ET(4,120)*/
*++tmpPtr = -0.1107519194E+00;    /* ET(5,120)*/
*++tmpPtr = -0.3142901137E-01;    /* ET(6,120)*/
*++tmpPtr = -0.2995752096E+00;    /* ET(7,120)*/
*++tmpPtr = -0.2412485890E-01;    /* ET(8,120)*/
*++tmpPtr = -0.1023615152E-01;    /* ET(9,120)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 121: */
 /* */
*++tmpPtr =  0.9715058208E+00;    /* ET(1,121)*/
*++tmpPtr =  0.1447042078E+00;    /* ET(2,121)*/
*++tmpPtr =  0.4068109393E+00;    /* ET(3,121)*/
*++tmpPtr = -0.7029048353E-01;    /* ET(4,121)*/
*++tmpPtr = -0.1976095885E+00;    /* ET(5,121)*/
*++tmpPtr = -0.2943362482E-01;    /* ET(6,121)*/
*++tmpPtr = -0.2359558791E+00;    /* ET(7,121)*/
*++tmpPtr = -0.5234826822E-02;    /* ET(8,121)*/
*++tmpPtr = -0.4137378559E-01;    /* ET(9,121)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 122: */
 /* */
*++tmpPtr =  0.1070786357E+01;    /* ET(1,122)*/
*++tmpPtr =  0.1518430561E+00;    /* ET(2,122)*/
*++tmpPtr =  0.3392490149E+00;    /* ET(3,122)*/
*++tmpPtr = -0.8129572868E-01;    /* ET(4,122)*/
*++tmpPtr = -0.1816315949E+00;    /* ET(5,122)*/
*++tmpPtr = -0.2575630136E-01;    /* ET(6,122)*/
*++tmpPtr = -0.2866458595E+00;    /* ET(7,122)*/
*++tmpPtr = -0.5764077883E-02;    /* ET(8,122)*/
*++tmpPtr = -0.2877247334E-01;    /* ET(9,122)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 123: */
 /* */
*++tmpPtr =  0.1074435234E+01;    /* ET(1,123)*/
*++tmpPtr =  0.1957409680E+00;    /* ET(2,123)*/
*++tmpPtr =  0.2333540022E+00;    /* ET(3,123)*/
*++tmpPtr = -0.1051554978E+00;    /* ET(4,123)*/
*++tmpPtr = -0.1253618747E+00;    /* ET(5,123)*/
*++tmpPtr = -0.2283846959E-01;    /* ET(6,123)*/
*++tmpPtr = -0.2886027396E+00;    /* ET(7,123)*/
*++tmpPtr = -0.9578632191E-02;    /* ET(8,123)*/
*++tmpPtr = -0.1361352205E-01;    /* ET(9,123)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 124: */
 /* */
*++tmpPtr =  0.1138250351E+01;    /* ET(1,124)*/
*++tmpPtr =  0.1275651902E+00;    /* ET(2,124)*/
*++tmpPtr =  0.1577568948E+00;    /* ET(3,124)*/
*++tmpPtr = -0.7260055840E-01;    /* ET(4,124)*/
*++tmpPtr = -0.8978342265E-01;    /* ET(5,124)*/
*++tmpPtr = -0.1006214321E-01;    /* ET(6,124)*/
*++tmpPtr = -0.3239034712E+00;    /* ET(7,124)*/
*++tmpPtr = -0.4068219103E-02;    /* ET(8,124)*/
*++tmpPtr = -0.6221808959E-02;    /* ET(9,124)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 125: */
 /* */
*++tmpPtr =  0.1183741331E+01;    /* ET(1,125)*/
*++tmpPtr =  0.1526151448E+00;    /* ET(2,125)*/
*++tmpPtr =  0.3998943269E+00;    /* ET(3,125)*/
*++tmpPtr = -0.9032842517E-01;    /* ET(4,125)*/
*++tmpPtr = -0.2366857082E+00;    /* ET(5,125)*/
*++tmpPtr = -0.3051496483E-01;    /* ET(6,125)*/
*++tmpPtr = -0.3503108919E+00;    /* ET(7,125)*/
*++tmpPtr = -0.5822845735E-02;    /* ET(8,125)*/
*++tmpPtr = -0.3997886553E-01;    /* ET(9,125)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 126: */
 /* */
*++tmpPtr =  0.1217068076E+01;    /* ET(1,126)*/
*++tmpPtr =  0.1444350332E+00;    /* ET(2,126)*/
*++tmpPtr =  0.2134053558E+00;    /* ET(3,126)*/
*++tmpPtr = -0.8789363503E-01;    /* ET(4,126)*/
*++tmpPtr = -0.1298644245E+00;    /* ET(5,126)*/
*++tmpPtr = -0.1541160699E-01;    /* ET(6,126)*/
*++tmpPtr = -0.3703136444E+00;    /* ET(7,126)*/
*++tmpPtr = -0.5215370096E-02;    /* ET(8,126)*/
*++tmpPtr = -0.1138546225E-01;    /* ET(9,126)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 127: */
 /* */
*++tmpPtr =  0.1308151245E+01;    /* ET(1,127)*/
*++tmpPtr =  0.1306512803E+00;    /* ET(2,127)*/
*++tmpPtr =  0.2444696873E+00;    /* ET(3,127)*/
*++tmpPtr = -0.8545581251E-01;    /* ET(4,127)*/
*++tmpPtr = -0.1599016488E+00;    /* ET(5,127)*/
*++tmpPtr = -0.1597013883E-01;    /* ET(6,127)*/
*++tmpPtr = -0.4278149009E+00;    /* ET(7,127)*/
*++tmpPtr = -0.4267438781E-02;    /* ET(8,127)*/
*++tmpPtr = -0.1494135708E-01;    /* ET(9,127)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 128: */
 /* */
*++tmpPtr =  0.1369695067E+01;    /* ET(1,128)*/
*++tmpPtr =  0.1145742089E+00;    /* ET(2,128)*/
*++tmpPtr =  0.1445382535E+00;    /* ET(3,128)*/
*++tmpPtr = -0.7846586406E-01;    /* ET(4,128)*/
*++tmpPtr = -0.9898667037E-01;    /* ET(5,128)*/
*++tmpPtr = -0.8280177601E-02;    /* ET(6,128)*/
*++tmpPtr = -0.4690161645E+00;    /* ET(7,128)*/
*++tmpPtr = -0.3281812416E-02;    /* ET(8,128)*/
*++tmpPtr = -0.5222826730E-02;    /* ET(9,128)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 129: */
 /* */
*++tmpPtr =  0.2258629799E+00;    /* ET(1,129)*/
*++tmpPtr =  0.8012950420E+00;    /* ET(2,129)*/
*++tmpPtr =  0.3608281612E+00;    /* ET(3,129)*/
*++tmpPtr = -0.9049144387E-01;    /* ET(4,129)*/
*++tmpPtr = -0.4074886069E-01;    /* ET(5,129)*/
*++tmpPtr = -0.1445648968E+00;    /* ET(6,129)*/
*++tmpPtr = -0.1275352109E-01;    /* ET(7,129)*/
*++tmpPtr = -0.1605184376E+00;    /* ET(8,129)*/
*++tmpPtr = -0.3254923970E-01;    /* ET(9,129)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 130: */
 /* */
*++tmpPtr =  0.3589501083E+00;    /* ET(1,130)*/
*++tmpPtr =  0.7719327211E+00;    /* ET(2,130)*/
*++tmpPtr =  0.5316622853E+00;    /* ET(3,130)*/
*++tmpPtr = -0.1385426670E+00;    /* ET(4,130)*/
*++tmpPtr = -0.9542011470E-01;    /* ET(5,130)*/
*++tmpPtr = -0.2052037269E+00;    /* ET(6,130)*/
*++tmpPtr = -0.3221129626E-01;    /* ET(7,130)*/
*++tmpPtr = -0.1489700228E+00;    /* ET(8,130)*/
*++tmpPtr = -0.7066619396E-01;    /* ET(9,130)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 131: */
 /* */
*++tmpPtr =  0.5622022152E+00;    /* ET(1,131)*/
*++tmpPtr =  0.7702801824E+00;    /* ET(2,131)*/
*++tmpPtr =  0.3292792439E+00;    /* ET(3,131)*/
*++tmpPtr = -0.2165265977E+00;    /* ET(4,131)*/
*++tmpPtr = -0.9256076068E-01;    /* ET(5,131)*/
*++tmpPtr = -0.1268186271E+00;    /* ET(6,131)*/
*++tmpPtr = -0.7901782542E-01;    /* ET(7,131)*/
*++tmpPtr = -0.1483328789E+00;    /* ET(8,131)*/
*++tmpPtr = -0.2710620500E-01;    /* ET(9,131)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 132: */
 /* */
*++tmpPtr =  0.6404845715E+00;    /* ET(1,132)*/
*++tmpPtr =  0.7653158307E+00;    /* ET(2,132)*/
*++tmpPtr =  0.5162186027E+00;    /* ET(3,132)*/
*++tmpPtr = -0.2450865060E+00;    /* ET(4,132)*/
*++tmpPtr = -0.1653150320E+00;    /* ET(5,132)*/
*++tmpPtr = -0.1975351274E+00;    /* ET(6,132)*/
*++tmpPtr = -0.1025551260E+00;    /* ET(7,132)*/
*++tmpPtr = -0.1464270800E+00;    /* ET(8,132)*/
*++tmpPtr = -0.6662040949E-01;    /* ET(9,132)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 133: */
 /* */
*++tmpPtr =  0.2577594817E+00;    /* ET(1,133)*/
*++tmpPtr =  0.1002155542E+01;    /* ET(2,133)*/
*++tmpPtr =  0.6620932817E+00;    /* ET(3,133)*/
*++tmpPtr = -0.1291575581E+00;    /* ET(4,133)*/
*++tmpPtr = -0.8533041179E-01;    /* ET(5,133)*/
*++tmpPtr = -0.3317602277E+00;    /* ET(6,133)*/
*++tmpPtr = -0.1660998911E-01;    /* ET(7,133)*/
*++tmpPtr = -0.2510789335E+00;    /* ET(8,133)*/
*++tmpPtr = -0.1095918790E+00;    /* ET(9,133)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 134: */
 /* */
*++tmpPtr =  0.3820424676E+00;    /* ET(1,134)*/
*++tmpPtr =  0.8326119184E+00;    /* ET(2,134)*/
*++tmpPtr =  0.9184875488E+00;    /* ET(3,134)*/
*++tmpPtr = -0.1590465605E+00;    /* ET(4,134)*/
*++tmpPtr = -0.1754506379E+00;    /* ET(5,134)*/
*++tmpPtr = -0.3823718429E+00;    /* ET(6,134)*/
*++tmpPtr = -0.3648911417E-01;    /* ET(7,134)*/
*++tmpPtr = -0.1733106524E+00;    /* ET(8,134)*/
*++tmpPtr = -0.2109048516E+00;    /* ET(9,134)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 135: */
 /* */
*++tmpPtr =  0.5652690530E+00;    /* ET(1,135)*/
*++tmpPtr =  0.8892942071E+00;    /* ET(2,135)*/
*++tmpPtr =  0.1003878832E+01;    /* ET(3,135)*/
*++tmpPtr = -0.2513452470E+00;    /* ET(4,135)*/
*++tmpPtr = -0.2837308049E+00;    /* ET(5,135)*/
*++tmpPtr = -0.4463717937E+00;    /* ET(6,135)*/
*++tmpPtr = -0.7988227159E-01;    /* ET(7,135)*/
*++tmpPtr = -0.1977110356E+00;    /* ET(8,135)*/
*++tmpPtr = -0.2519431710E+00;    /* ET(9,135)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 136: */
 /* */
*++tmpPtr =  0.6765009761E+00;    /* ET(1,136)*/
*++tmpPtr =  0.8613968492E+00;    /* ET(2,136)*/
*++tmpPtr =  0.1433642864E+01;    /* ET(3,136)*/
*++tmpPtr = -0.2913679183E+00;    /* ET(4,136)*/
*++tmpPtr = -0.4849303961E+00;    /* ET(5,136)*/
*++tmpPtr = -0.6174677014E+00;    /* ET(6,136)*/
*++tmpPtr = -0.1144133955E+00;    /* ET(7,136)*/
*++tmpPtr = -0.1855011433E+00;    /* ET(8,136)*/
*++tmpPtr = -0.5138329864E+00;    /* ET(9,136)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 137: */
 /* */
*++tmpPtr =  0.5723449588E+00;    /* ET(1,137)*/
*++tmpPtr =  0.6671476364E+00;    /* ET(2,137)*/
*++tmpPtr =  0.6986287236E+00;    /* ET(3,137)*/
*++tmpPtr = -0.1909192801E+00;    /* ET(4,137)*/
*++tmpPtr = -0.1999282986E+00;    /* ET(5,137)*/
*++tmpPtr = -0.2330442518E+00;    /* ET(6,137)*/
*++tmpPtr = -0.8189468086E-01;    /* ET(7,137)*/
*++tmpPtr = -0.1112715006E+00;    /* ET(8,137)*/
*++tmpPtr = -0.1220205277E+00;    /* ET(9,137)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 138: */
 /* */
*++tmpPtr =  0.7224833369E+00;    /* ET(1,138)*/
*++tmpPtr =  0.5949907899E+00;    /* ET(2,138)*/
*++tmpPtr =  0.7009717226E+00;    /* ET(3,138)*/
*++tmpPtr = -0.2149354815E+00;    /* ET(4,138)*/
*++tmpPtr = -0.2532202005E+00;    /* ET(5,138)*/
*++tmpPtr = -0.2085358799E+00;    /* ET(6,138)*/
*++tmpPtr = -0.1304955482E+00;    /* ET(7,138)*/
*++tmpPtr = -0.8850351721E-01;    /* ET(8,138)*/
*++tmpPtr = -0.1228403449E+00;    /* ET(9,138)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 139: */
 /* */
*++tmpPtr =  0.7606726289E+00;    /* ET(1,139)*/
*++tmpPtr =  0.7556130886E+00;    /* ET(2,139)*/
*++tmpPtr =  0.6815907955E+00;    /* ET(3,139)*/
*++tmpPtr = -0.2873871028E+00;    /* ET(4,139)*/
*++tmpPtr = -0.2592337430E+00;    /* ET(5,139)*/
*++tmpPtr = -0.2575094700E+00;    /* ET(6,139)*/
*++tmpPtr = -0.1446557194E+00;    /* ET(7,139)*/
*++tmpPtr = -0.1427377909E+00;    /* ET(8,139)*/
*++tmpPtr = -0.1161415055E+00;    /* ET(9,139)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 140: */
 /* */
*++tmpPtr =  0.9004931450E+00;    /* ET(1,140)*/
*++tmpPtr =  0.7673043013E+00;    /* ET(2,140)*/
*++tmpPtr =  0.7945219874E+00;    /* ET(3,140)*/
*++tmpPtr = -0.3454760909E+00;    /* ET(4,140)*/
*++tmpPtr = -0.3577307761E+00;    /* ET(5,140)*/
*++tmpPtr = -0.3048200309E+00;    /* ET(6,140)*/
*++tmpPtr = -0.2027219683E+00;    /* ET(7,140)*/
*++tmpPtr = -0.1471889615E+00;    /* ET(8,140)*/
*++tmpPtr = -0.1578162909E+00;    /* ET(9,140)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 141: */
 /* */
*++tmpPtr =  0.7484335303E+00;    /* ET(1,141)*/
*++tmpPtr =  0.6358241439E+00;    /* ET(2,141)*/
*++tmpPtr =  0.9616393447E+00;    /* ET(3,141)*/
*++tmpPtr = -0.2379360497E+00;    /* ET(4,141)*/
*++tmpPtr = -0.3598615527E+00;    /* ET(5,141)*/
*++tmpPtr = -0.3057167530E+00;    /* ET(6,141)*/
*++tmpPtr = -0.1400381774E+00;    /* ET(7,141)*/
*++tmpPtr = -0.1010680869E+00;    /* ET(8,141)*/
*++tmpPtr = -0.2311875522E+00;    /* ET(9,141)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 142: */
 /* */
*++tmpPtr =  0.9552582502E+00;    /* ET(1,142)*/
*++tmpPtr =  0.5823326707E+00;    /* ET(2,142)*/
*++tmpPtr =  0.1006715894E+01;    /* ET(3,142)*/
*++tmpPtr = -0.2781390250E+00;    /* ET(4,142)*/
*++tmpPtr = -0.4808368087E+00;    /* ET(5,142)*/
*++tmpPtr = -0.2931217551E+00;    /* ET(6,142)*/
*++tmpPtr = -0.2281295806E+00;    /* ET(7,142)*/
*++tmpPtr = -0.8477783948E-01;    /* ET(8,142)*/
*++tmpPtr = -0.2533692122E+00;    /* ET(9,142)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 143: */
 /* */
*++tmpPtr =  0.1046293855E+01;    /* ET(1,143)*/
*++tmpPtr =  0.7393320799E+00;    /* ET(2,143)*/
*++tmpPtr =  0.1056529880E+01;    /* ET(3,143)*/
*++tmpPtr = -0.3867792785E+00;    /* ET(4,143)*/
*++tmpPtr = -0.5527203679E+00;    /* ET(5,143)*/
*++tmpPtr = -0.3905631900E+00;    /* ET(6,143)*/
*++tmpPtr = -0.2736827135E+00;    /* ET(7,143)*/
*++tmpPtr = -0.1366529763E+00;    /* ET(8,143)*/
*++tmpPtr = -0.2790638208E+00;    /* ET(9,143)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 144: */
 /* */
*++tmpPtr =  0.1294261694E+01;    /* ET(1,144)*/
*++tmpPtr =  0.6717921495E+00;    /* ET(2,144)*/
*++tmpPtr =  0.1151122332E+01;    /* ET(3,144)*/
*++tmpPtr = -0.4347374439E+00;    /* ET(4,144)*/
*++tmpPtr = -0.7449267507E+00;    /* ET(5,144)*/
*++tmpPtr = -0.3866574764E+00;    /* ET(6,144)*/
*++tmpPtr = -0.4187783003E+00;    /* ET(7,144)*/
*++tmpPtr = -0.1128261760E+00;    /* ET(8,144)*/
*++tmpPtr = -0.3312706649E+00;    /* ET(9,144)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 145: */
 /* */
*++tmpPtr =  0.5417724252E+00;    /* ET(1,145)*/
*++tmpPtr =  0.1046983957E+01;    /* ET(2,145)*/
*++tmpPtr =  0.4782333672E+00;    /* ET(3,145)*/
*++tmpPtr = -0.2836135328E+00;    /* ET(4,145)*/
*++tmpPtr = -0.1295468360E+00;    /* ET(5,145)*/
*++tmpPtr = -0.2503513396E+00;    /* ET(6,145)*/
*++tmpPtr = -0.7337934524E-01;    /* ET(7,145)*/
*++tmpPtr = -0.2740438581E+00;    /* ET(8,145)*/
*++tmpPtr = -0.5717678741E-01;    /* ET(9,145)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 146: */
 /* */
*++tmpPtr =  0.7067283988E+00;    /* ET(1,146)*/
*++tmpPtr =  0.9596821666E+00;    /* ET(2,146)*/
*++tmpPtr =  0.6746466160E+00;    /* ET(3,146)*/
*++tmpPtr = -0.3391173184E+00;    /* ET(4,146)*/
*++tmpPtr = -0.2383959442E+00;    /* ET(5,146)*/
*++tmpPtr = -0.3237231672E+00;    /* ET(6,146)*/
*++tmpPtr = -0.1248662472E+00;    /* ET(7,146)*/
*++tmpPtr = -0.2302474678E+00;    /* ET(8,146)*/
*++tmpPtr = -0.1137870178E+00;    /* ET(9,146)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 147: */
 /* */
*++tmpPtr =  0.9145875573E+00;    /* ET(1,147)*/
*++tmpPtr =  0.9344299436E+00;    /* ET(2,147)*/
*++tmpPtr =  0.5153293610E+00;    /* ET(3,147)*/
*++tmpPtr = -0.4273090065E+00;    /* ET(4,147)*/
*++tmpPtr = -0.2356568873E+00;    /* ET(5,147)*/
*++tmpPtr = -0.2407695949E+00;    /* ET(6,147)*/
*++tmpPtr = -0.2091175914E+00;    /* ET(7,147)*/
*++tmpPtr = -0.2182898372E+00;    /* ET(8,147)*/
*++tmpPtr = -0.6639108062E-01;    /* ET(9,147)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 148: */
 /* */
*++tmpPtr =  0.1153149843E+01;    /* ET(1,148)*/
*++tmpPtr =  0.9785483479E+00;    /* ET(2,148)*/
*++tmpPtr =  0.5351203084E+00;    /* ET(3,148)*/
*++tmpPtr = -0.5642064214E+00;    /* ET(4,148)*/
*++tmpPtr = -0.3085369468E+00;    /* ET(5,148)*/
*++tmpPtr = -0.2618205249E+00;    /* ET(6,148)*/
*++tmpPtr = -0.3324386477E+00;    /* ET(7,148)*/
*++tmpPtr = -0.2393892109E+00;    /* ET(8,148)*/
*++tmpPtr = -0.7158843428E-01;    /* ET(9,148)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 149: */
 /* */
*++tmpPtr =  0.5050533414E+00;    /* ET(1,149)*/
*++tmpPtr =  0.1276940227E+01;    /* ET(2,149)*/
*++tmpPtr =  0.8056992888E+00;    /* ET(3,149)*/
*++tmpPtr = -0.3224614561E+00;    /* ET(4,149)*/
*++tmpPtr = -0.2034605443E+00;    /* ET(5,149)*/
*++tmpPtr = -0.5144149065E+00;    /* ET(6,149)*/
*++tmpPtr = -0.6376971304E-01;    /* ET(7,149)*/
*++tmpPtr = -0.4076441228E+00;    /* ET(8,149)*/
*++tmpPtr = -0.1622878313E+00;    /* ET(9,149)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 150: */
 /* */
*++tmpPtr =  0.8132929802E+00;    /* ET(1,150)*/
*++tmpPtr =  0.1158025503E+01;    /* ET(2,150)*/
*++tmpPtr =  0.9521093965E+00;    /* ET(3,150)*/
*++tmpPtr = -0.4709070325E+00;    /* ET(4,150)*/
*++tmpPtr = -0.3871719539E+00;    /* ET(5,150)*/
*++tmpPtr = -0.5512834191E+00;    /* ET(6,150)*/
*++tmpPtr = -0.1653613746E+00;    /* ET(7,150)*/
*++tmpPtr = -0.3352557421E+00;    /* ET(8,150)*/
*++tmpPtr = -0.2266280800E+00;    /* ET(9,150)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 151: */
 /* */
*++tmpPtr =  0.1000128865E+01;    /* ET(1,151)*/
*++tmpPtr =  0.9345731139E+00;    /* ET(2,151)*/
*++tmpPtr =  0.8819470406E+00;    /* ET(3,151)*/
*++tmpPtr = -0.4673468173E+00;    /* ET(4,151)*/
*++tmpPtr = -0.4410303533E+00;    /* ET(5,151)*/
*++tmpPtr = -0.4121219814E+00;    /* ET(6,151)*/
*++tmpPtr = -0.2500644624E+00;    /* ET(7,151)*/
*++tmpPtr = -0.2183567286E+00;    /* ET(8,151)*/
*++tmpPtr = -0.1944576353E+00;    /* ET(9,151)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 152: */
 /* */
*++tmpPtr =  0.1232571602E+01;    /* ET(1,152)*/
*++tmpPtr =  0.1065505981E+01;    /* ET(2,152)*/
*++tmpPtr =  0.8852382898E+00;    /* ET(3,152)*/
*++tmpPtr = -0.6566562057E+00;    /* ET(4,152)*/
*++tmpPtr = -0.5455597639E+00;    /* ET(5,152)*/
*++tmpPtr = -0.4716133177E+00;    /* ET(6,152)*/
*++tmpPtr = -0.3798081875E+00;    /* ET(7,152)*/
*++tmpPtr = -0.2838257253E+00;    /* ET(8,152)*/
*++tmpPtr = -0.1959117055E+00;    /* ET(9,152)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 153: */
 /* */
*++tmpPtr =  0.1004501581E+01;    /* ET(1,153)*/
*++tmpPtr =  0.7032595277E+00;    /* ET(2,153)*/
*++tmpPtr =  0.6344028711E+00;    /* ET(3,153)*/
*++tmpPtr = -0.3532126248E+00;    /* ET(4,153)*/
*++tmpPtr = -0.3186293542E+00;    /* ET(5,153)*/
*++tmpPtr = -0.2230749279E+00;    /* ET(6,153)*/
*++tmpPtr = -0.2522558570E+00;    /* ET(7,153)*/
*++tmpPtr = -0.1236434877E+00;    /* ET(8,153)*/
*++tmpPtr = -0.1006167531E+00;    /* ET(9,153)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 154: */
 /* */
*++tmpPtr =  0.1164071560E+01;    /* ET(1,154)*/
*++tmpPtr =  0.6632366776E+00;    /* ET(2,154)*/
*++tmpPtr =  0.6778370738E+00;    /* ET(3,154)*/
*++tmpPtr = -0.3860274553E+00;    /* ET(4,154)*/
*++tmpPtr = -0.3945253789E+00;    /* ET(5,154)*/
*++tmpPtr = -0.2247831821E+00;    /* ET(6,154)*/
*++tmpPtr = -0.3387656212E+00;    /* ET(7,154)*/
*++tmpPtr = -0.1099707186E+00;    /* ET(8,154)*/
*++tmpPtr = -0.1148657650E+00;    /* ET(9,154)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 155: */
 /* */
*++tmpPtr =  0.1205092669E+01;    /* ET(1,155)*/
*++tmpPtr =  0.7233220935E+00;    /* ET(2,155)*/
*++tmpPtr =  0.4782495201E+00;    /* ET(3,155)*/
*++tmpPtr = -0.4358350337E+00;    /* ET(4,155)*/
*++tmpPtr = -0.2881674767E+00;    /* ET(5,155)*/
*++tmpPtr = -0.1729642153E+00;    /* ET(6,155)*/
*++tmpPtr = -0.3630620539E+00;    /* ET(7,155)*/
*++tmpPtr = -0.1307987124E+00;    /* ET(8,155)*/
*++tmpPtr = -0.5718065053E-01;    /* ET(9,155)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 156: */
 /* */
*++tmpPtr =  0.1388947248E+01;    /* ET(1,156)*/
*++tmpPtr =  0.6694558263E+00;    /* ET(2,156)*/
*++tmpPtr =  0.4285798669E+00;    /* ET(3,156)*/
*++tmpPtr = -0.4649193883E+00;    /* ET(4,156)*/
*++tmpPtr = -0.2976374030E+00;    /* ET(5,156)*/
*++tmpPtr = -0.1434576362E+00;    /* ET(6,156)*/
*++tmpPtr = -0.4822935760E+00;    /* ET(7,156)*/
*++tmpPtr = -0.1120427847E+00;    /* ET(8,156)*/
*++tmpPtr = -0.4592017457E-01;    /* ET(9,156)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 157: */
 /* */
*++tmpPtr =  0.1279690027E+01;    /* ET(1,157)*/
*++tmpPtr =  0.8156756163E+00;    /* ET(2,157)*/
*++tmpPtr =  0.8282577395E+00;    /* ET(3,157)*/
*++tmpPtr = -0.5219059587E+00;    /* ET(4,157)*/
*++tmpPtr = -0.5299565196E+00;    /* ET(5,157)*/
*++tmpPtr = -0.3377948105E+00;    /* ET(6,157)*/
*++tmpPtr = -0.4094016254E+00;    /* ET(7,157)*/
*++tmpPtr = -0.1663316786E+00;    /* ET(8,157)*/
*++tmpPtr = -0.1715027094E+00;    /* ET(9,157)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 158: */
 /* */
*++tmpPtr =  0.1474096060E+01;    /* ET(1,158)*/
*++tmpPtr =  0.8199666739E+00;    /* ET(2,158)*/
*++tmpPtr =  0.6825483441E+00;    /* ET(3,158)*/
*++tmpPtr = -0.6043548584E+00;    /* ET(4,158)*/
*++tmpPtr = -0.5030708909E+00;    /* ET(5,158)*/
*++tmpPtr = -0.2798334658E+00;    /* ET(6,158)*/
*++tmpPtr = -0.5432397723E+00;    /* ET(7,158)*/
*++tmpPtr = -0.1680863351E+00;    /* ET(8,158)*/
*++tmpPtr = -0.1164680645E+00;    /* ET(9,158)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 159: */
 /* */
*++tmpPtr =  0.1702578425E+01;    /* ET(1,159)*/
*++tmpPtr =  0.1006380677E+01;    /* ET(2,159)*/
*++tmpPtr =  0.7515375614E+00;    /* ET(3,159)*/
*++tmpPtr = -0.8567210436E+00;    /* ET(4,159)*/
*++tmpPtr = -0.6397758126E+00;    /* ET(5,159)*/
*++tmpPtr = -0.3781664073E+00;    /* ET(6,159)*/
*++tmpPtr = -0.7246933579E+00;    /* ET(7,159)*/
*++tmpPtr = -0.2532005012E+00;    /* ET(8,159)*/
*++tmpPtr = -0.1412021667E+00;    /* ET(9,159)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 160: */
 /* */
*++tmpPtr =  0.1866660833E+01;    /* ET(1,160)*/
*++tmpPtr =  0.8324823976E+00;    /* ET(2,160)*/
*++tmpPtr =  0.5605094433E+00;    /* ET(3,160)*/
*++tmpPtr = -0.7769811153E+00;    /* ET(4,160)*/
*++tmpPtr = -0.5231404901E+00;    /* ET(5,160)*/
*++tmpPtr = -0.2333071083E+00;    /* ET(6,160)*/
*++tmpPtr = -0.8711056113E+00;    /* ET(7,160)*/
*++tmpPtr = -0.1732567251E+00;    /* ET(8,160)*/
*++tmpPtr = -0.7854270190E-01;    /* ET(9,160)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 161: */
 /* */
*++tmpPtr =  0.6254309416E+00;    /* ET(1,161)*/
*++tmpPtr =  0.4197660983E+00;    /* ET(2,161)*/
*++tmpPtr =  0.5739719272E+00;    /* ET(3,161)*/
*++tmpPtr = -0.1312673539E+00;    /* ET(4,161)*/
*++tmpPtr = -0.1794898957E+00;    /* ET(5,161)*/
*++tmpPtr = -0.1204669774E+00;    /* ET(6,161)*/
*++tmpPtr = -0.9779095650E-01;    /* ET(7,161)*/
*++tmpPtr = -0.4405089468E-01;    /* ET(8,161)*/
*++tmpPtr = -0.8236094564E-01;    /* ET(9,161)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 162: */
 /* */
*++tmpPtr =  0.7736748457E+00;    /* ET(1,162)*/
*++tmpPtr =  0.4214665890E+00;    /* ET(2,162)*/
*++tmpPtr =  0.5417234302E+00;    /* ET(3,162)*/
*++tmpPtr = -0.1630390584E+00;    /* ET(4,162)*/
*++tmpPtr = -0.2095589042E+00;    /* ET(5,162)*/
*++tmpPtr = -0.1141591668E+00;    /* ET(6,162)*/
*++tmpPtr = -0.1496431977E+00;    /* ET(7,162)*/
*++tmpPtr = -0.4440852255E-01;    /* ET(8,162)*/
*++tmpPtr = -0.7336606830E-01;    /* ET(9,162)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 163: */
 /* */
*++tmpPtr =  0.8232557178E+00;    /* ET(1,163)*/
*++tmpPtr =  0.3457930386E+00;    /* ET(2,163)*/
*++tmpPtr =  0.6667904854E+00;    /* ET(3,163)*/
*++tmpPtr = -0.1423380524E+00;    /* ET(4,163)*/
*++tmpPtr = -0.2744695544E+00;    /* ET(5,163)*/
*++tmpPtr = -0.1152857542E+00;    /* ET(6,163)*/
*++tmpPtr = -0.1694374979E+00;    /* ET(7,163)*/
*++tmpPtr = -0.2989320457E-01;    /* ET(8,163)*/
*++tmpPtr = -0.1111523956E+00;    /* ET(9,163)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 164: */
 /* */
*++tmpPtr =  0.9484627247E+00;    /* ET(1,164)*/
*++tmpPtr =  0.3752231300E+00;    /* ET(2,164)*/
*++tmpPtr =  0.5395948291E+00;    /* ET(3,164)*/
*++tmpPtr = -0.1779425591E+00;    /* ET(4,164)*/
*++tmpPtr = -0.2558927834E+00;    /* ET(5,164)*/
*++tmpPtr = -0.1012342274E+00;    /* ET(6,164)*/
*++tmpPtr = -0.2248953730E+00;    /* ET(7,164)*/
*++tmpPtr = -0.3519809991E-01;    /* ET(8,164)*/
*++tmpPtr = -0.7279063761E-01;    /* ET(9,164)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 165: */
 /* */
*++tmpPtr =  0.8708004355E+00;    /* ET(1,165)*/
*++tmpPtr =  0.5244134665E+00;    /* ET(2,165)*/
*++tmpPtr =  0.6744532585E+00;    /* ET(3,165)*/
*++tmpPtr = -0.2283297330E+00;    /* ET(4,165)*/
*++tmpPtr = -0.2936570942E+00;    /* ET(5,165)*/
*++tmpPtr = -0.1768461913E+00;    /* ET(6,165)*/
*++tmpPtr = -0.1895733476E+00;    /* ET(7,165)*/
*++tmpPtr = -0.6875237077E-01;    /* ET(8,165)*/
*++tmpPtr = -0.1137217954E+00;    /* ET(9,165)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 166: */
 /* */
*++tmpPtr =  0.9929822087E+00;    /* ET(1,166)*/
*++tmpPtr =  0.5476214290E+00;    /* ET(2,166)*/
*++tmpPtr =  0.6482622027E+00;    /* ET(3,166)*/
*++tmpPtr = -0.2718891799E+00;    /* ET(4,166)*/
*++tmpPtr = -0.3218564391E+00;    /* ET(5,166)*/
*++tmpPtr = -0.1775011569E+00;    /* ET(6,166)*/
*++tmpPtr = -0.2465034127E+00;    /* ET(7,166)*/
*++tmpPtr = -0.7497230917E-01;    /* ET(8,166)*/
*++tmpPtr = -0.1050609797E+00;    /* ET(9,166)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 167: */
 /* */
*++tmpPtr =  0.1062217951E+01;    /* ET(1,167)*/
*++tmpPtr =  0.4250227213E+00;    /* ET(2,167)*/
*++tmpPtr =  0.6588529348E+00;    /* ET(3,167)*/
*++tmpPtr = -0.2257333696E+00;    /* ET(4,167)*/
*++tmpPtr = -0.3499227166E+00;    /* ET(5,167)*/
*++tmpPtr = -0.1400137395E+00;    /* ET(6,167)*/
*++tmpPtr = -0.2820767164E+00;    /* ET(7,167)*/
*++tmpPtr = -0.4516107962E-01;    /* ET(8,167)*/
*++tmpPtr = -0.1085218042E+00;    /* ET(9,167)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 168: */
 /* */
*++tmpPtr =  0.1173936844E+01;    /* ET(1,168)*/
*++tmpPtr =  0.4879429638E+00;    /* ET(2,168)*/
*++tmpPtr =  0.5714526772E+00;    /* ET(3,168)*/
*++tmpPtr = -0.2864070833E+00;    /* ET(4,168)*/
*++tmpPtr = -0.3354246616E+00;    /* ET(5,168)*/
*++tmpPtr = -0.1394181550E+00;    /* ET(6,168)*/
*++tmpPtr = -0.3445318937E+00;    /* ET(7,168)*/
*++tmpPtr = -0.5952208117E-01;    /* ET(8,168)*/
*++tmpPtr = -0.8163953573E-01;    /* ET(9,168)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 169: */
 /* */
*++tmpPtr =  0.9231903553E+00;    /* ET(1,169)*/
*++tmpPtr =  0.4720732868E+00;    /* ET(2,169)*/
*++tmpPtr =  0.4344846010E+00;    /* ET(3,169)*/
*++tmpPtr = -0.2179067433E+00;    /* ET(4,169)*/
*++tmpPtr = -0.2005559951E+00;    /* ET(5,169)*/
*++tmpPtr = -0.1025542915E+00;    /* ET(6,169)*/
*++tmpPtr = -0.2130700946E+00;    /* ET(7,169)*/
*++tmpPtr = -0.5571329966E-01;    /* ET(8,169)*/
*++tmpPtr = -0.4719421640E-01;    /* ET(9,169)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 170: */
 /* */
*++tmpPtr =  0.1033950090E+01;    /* ET(1,170)*/
*++tmpPtr =  0.4453985095E+00;    /* ET(2,170)*/
*++tmpPtr =  0.3798229694E+00;    /* ET(3,170)*/
*++tmpPtr = -0.2302599102E+00;    /* ET(4,170)*/
*++tmpPtr = -0.1963589787E+00;    /* ET(5,170)*/
*++tmpPtr = -0.8458629251E-01;    /* ET(6,170)*/
*++tmpPtr = -0.2672631741E+00;    /* ET(7,170)*/
*++tmpPtr = -0.4959496111E-01;    /* ET(8,170)*/
*++tmpPtr = -0.3606637195E-01;    /* ET(9,170)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 171: */
 /* */
*++tmpPtr =  0.1121427059E+01;    /* ET(1,171)*/
*++tmpPtr =  0.4981115460E+00;    /* ET(2,171)*/
*++tmpPtr =  0.3489298224E+00;    /* ET(3,171)*/
*++tmpPtr = -0.2792978585E+00;    /* ET(4,171)*/
*++tmpPtr = -0.1956496686E+00;    /* ET(5,171)*/
*++tmpPtr = -0.8690298349E-01;    /* ET(6,171)*/
*++tmpPtr = -0.3143996298E+00;    /* ET(7,171)*/
*++tmpPtr = -0.6202877685E-01;    /* ET(8,171)*/
*++tmpPtr = -0.3043800779E-01;    /* ET(9,171)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 172: */
 /* */
*++tmpPtr =  0.1190706491E+01;    /* ET(1,172)*/
*++tmpPtr =  0.4506414831E+00;    /* ET(2,172)*/
*++tmpPtr =  0.2276199460E+00;    /* ET(3,172)*/
*++tmpPtr = -0.2682908475E+00;    /* ET(4,172)*/
*++tmpPtr = -0.1355142593E+00;    /* ET(5,172)*/
*++tmpPtr = -0.5128749460E-01;    /* ET(6,172)*/
*++tmpPtr = -0.3544454575E+00;    /* ET(7,172)*/
*++tmpPtr = -0.5076943338E-01;    /* ET(8,172)*/
*++tmpPtr = -0.1295270957E-01;    /* ET(9,172)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 173: */
 /* */
*++tmpPtr =  0.1167983294E+01;    /* ET(1,173)*/
*++tmpPtr =  0.3802623451E+00;    /* ET(2,173)*/
*++tmpPtr =  0.4540638328E+00;    /* ET(3,173)*/
*++tmpPtr = -0.2220700532E+00;    /* ET(4,173)*/
*++tmpPtr = -0.2651695013E+00;    /* ET(5,173)*/
*++tmpPtr = -0.8633169532E-01;    /* ET(6,173)*/
*++tmpPtr = -0.3410462439E+00;    /* ET(7,173)*/
*++tmpPtr = -0.3614986315E-01;    /* ET(8,173)*/
*++tmpPtr = -0.5154349282E-01;    /* ET(9,173)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 174: */
 /* */
*++tmpPtr =  0.1271537304E+01;    /* ET(1,174)*/
*++tmpPtr =  0.3771513402E+00;    /* ET(2,174)*/
*++tmpPtr =  0.3636589050E+00;    /* ET(3,174)*/
*++tmpPtr = -0.2397810072E+00;    /* ET(4,174)*/
*++tmpPtr = -0.2312029451E+00;    /* ET(5,174)*/
*++tmpPtr = -0.6857721508E-01;    /* ET(6,174)*/
*++tmpPtr = -0.4042017758E+00;    /* ET(7,174)*/
*++tmpPtr = -0.3556078300E-01;    /* ET(8,174)*/
*++tmpPtr = -0.3306194767E-01;    /* ET(9,174)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 175: */
 /* */
*++tmpPtr =  0.1350750685E+01;    /* ET(1,175)*/
*++tmpPtr =  0.4673993289E+00;    /* ET(2,175)*/
*++tmpPtr =  0.3528914452E+00;    /* ET(3,175)*/
*++tmpPtr = -0.3156699538E+00;    /* ET(4,175)*/
*++tmpPtr = -0.2383341640E+00;    /* ET(5,175)*/
*++tmpPtr = -0.8247061074E-01;    /* ET(6,175)*/
*++tmpPtr = -0.4561318159E+00;    /* ET(7,175)*/
*++tmpPtr = -0.5461553112E-01;    /* ET(8,175)*/
*++tmpPtr = -0.3113309108E-01;    /* ET(9,175)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 176: */
 /* */
*++tmpPtr =  0.1425765753E+01;    /* ET(1,176)*/
*++tmpPtr =  0.3741326928E+00;    /* ET(2,176)*/
*++tmpPtr =  0.2402400821E+00;    /* ET(3,176)*/
*++tmpPtr = -0.2667127848E+00;    /* ET(4,176)*/
*++tmpPtr = -0.1712630242E+00;    /* ET(5,176)*/
*++tmpPtr = -0.4494083673E-01;    /* ET(6,176)*/
*++tmpPtr = -0.5082019567E+00;    /* ET(7,176)*/
*++tmpPtr = -0.3499381989E-01;    /* ET(8,176)*/
*++tmpPtr = -0.1442882419E-01;    /* ET(9,176)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 177: */
 /* */
*++tmpPtr =  0.1069266915E+01;    /* ET(1,177)*/
*++tmpPtr =  0.2534427047E+00;    /* ET(2,177)*/
*++tmpPtr =  0.6173679233E+00;    /* ET(3,177)*/
*++tmpPtr = -0.1354989409E+00;    /* ET(4,177)*/
*++tmpPtr = -0.3300655484E+00;    /* ET(5,177)*/
*++tmpPtr = -0.7823369652E-01;    /* ET(6,177)*/
*++tmpPtr = -0.2858329117E+00;    /* ET(7,177)*/
*++tmpPtr = -0.1605830155E-01;    /* ET(8,177)*/
*++tmpPtr = -0.9528578818E-01;    /* ET(9,177)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 178: */
 /* */
*++tmpPtr =  0.1139601469E+01;    /* ET(1,178)*/
*++tmpPtr =  0.2602315843E+00;    /* ET(2,178)*/
*++tmpPtr =  0.4505327642E+00;    /* ET(3,178)*/
*++tmpPtr = -0.1482801437E+00;    /* ET(4,178)*/
*++tmpPtr = -0.2567138672E+00;    /* ET(5,178)*/
*++tmpPtr = -0.5862142891E-01;    /* ET(6,178)*/
*++tmpPtr = -0.3246728480E+00;    /* ET(7,178)*/
*++tmpPtr = -0.1693012007E-01;    /* ET(8,178)*/
*++tmpPtr = -0.5074494332E-01;    /* ET(9,178)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 179: */
 /* */
*++tmpPtr =  0.1236974597E+01;    /* ET(1,179)*/
*++tmpPtr =  0.2431111634E+00;    /* ET(2,179)*/
*++tmpPtr =  0.5888975859E+00;    /* ET(3,179)*/
*++tmpPtr = -0.1503611654E+00;    /* ET(4,179)*/
*++tmpPtr = -0.3642256856E+00;    /* ET(5,179)*/
*++tmpPtr = -0.7158379257E-01;    /* ET(6,179)*/
*++tmpPtr = -0.3825265467E+00;    /* ET(7,179)*/
*++tmpPtr = -0.1477575954E-01;    /* ET(8,179)*/
*++tmpPtr = -0.8670009673E-01;    /* ET(9,179)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 180: */
 /* */
*++tmpPtr =  0.1323566794E+01;    /* ET(1,180)*/
*++tmpPtr =  0.2792697549E+00;    /* ET(2,180)*/
*++tmpPtr =  0.4271676242E+00;    /* ET(3,180)*/
*++tmpPtr = -0.1848160774E+00;    /* ET(4,180)*/
*++tmpPtr = -0.2826924324E+00;    /* ET(5,180)*/
*++tmpPtr = -0.5964749679E-01;    /* ET(6,180)*/
*++tmpPtr = -0.4379572868E+00;    /* ET(7,180)*/
*++tmpPtr = -0.1949789934E-01;    /* ET(8,180)*/
*++tmpPtr = -0.4561804235E-01;    /* ET(9,180)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 181: */
 /* */
*++tmpPtr =  0.1176199913E+01;    /* ET(1,181)*/
*++tmpPtr =  0.2662818134E+00;    /* ET(2,181)*/
*++tmpPtr =  0.3252506256E+00;    /* ET(3,181)*/
*++tmpPtr = -0.1566003114E+00;    /* ET(4,181)*/
*++tmpPtr = -0.1912798733E+00;    /* ET(5,181)*/
*++tmpPtr = -0.4330416396E-01;    /* ET(6,181)*/
*++tmpPtr = -0.3458615243E+00;    /* ET(7,181)*/
*++tmpPtr = -0.1772649959E-01;    /* ET(8,181)*/
*++tmpPtr = -0.2644699439E-01;    /* ET(9,181)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 182: */
 /* */
*++tmpPtr =  0.1225716710E+01;    /* ET(1,182)*/
*++tmpPtr =  0.2650870085E+00;    /* ET(2,182)*/
*++tmpPtr =  0.2137087435E+00;    /* ET(3,182)*/
*++tmpPtr = -0.1624607742E+00;    /* ET(4,182)*/
*++tmpPtr = -0.1309732050E+00;    /* ET(5,182)*/
*++tmpPtr = -0.2832570486E-01;    /* ET(6,182)*/
*++tmpPtr = -0.3755953610E+00;    /* ET(7,182)*/
*++tmpPtr = -0.1756777987E-01;    /* ET(8,182)*/
*++tmpPtr = -0.1141785737E-01;    /* ET(9,182)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 183: */
 /* */
*++tmpPtr =  0.1312647223E+01;    /* ET(1,183)*/
*++tmpPtr =  0.2401364148E+00;    /* ET(2,183)*/
*++tmpPtr =  0.2924692929E+00;    /* ET(3,183)*/
*++tmpPtr = -0.1576071829E+00;    /* ET(4,183)*/
*++tmpPtr = -0.1919544786E+00;    /* ET(5,183)*/
*++tmpPtr = -0.3511626273E-01;    /* ET(6,183)*/
*++tmpPtr = -0.4307606518E+00;    /* ET(7,183)*/
*++tmpPtr = -0.1441637427E-01;    /* ET(8,183)*/
*++tmpPtr = -0.2138457261E-01;    /* ET(9,183)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 184: */
 /* */
*++tmpPtr =  0.1352827311E+01;    /* ET(1,184)*/
*++tmpPtr =  0.2391681671E+00;    /* ET(2,184)*/
*++tmpPtr =  0.1783758402E+00;    /* ET(3,184)*/
*++tmpPtr = -0.1617766321E+00;    /* ET(4,184)*/
*++tmpPtr = -0.1206558570E+00;    /* ET(5,184)*/
*++tmpPtr = -0.2133091353E-01;    /* ET(6,184)*/
*++tmpPtr = -0.4575354457E+00;    /* ET(7,184)*/
*++tmpPtr = -0.1430035383E-01;    /* ET(8,184)*/
*++tmpPtr = -0.7954485714E-02;    /* ET(9,184)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 185: */
 /* */
*++tmpPtr =  0.1305015206E+01;    /* ET(1,185)*/
*++tmpPtr =  0.1548215449E+00;    /* ET(2,185)*/
*++tmpPtr =  0.4258761406E+00;    /* ET(3,185)*/
*++tmpPtr = -0.1010222360E+00;    /* ET(4,185)*/
*++tmpPtr = -0.2778874040E+00;    /* ET(5,185)*/
*++tmpPtr = -0.3296740353E-01;    /* ET(6,185)*/
*++tmpPtr = -0.4257661402E+00;    /* ET(7,185)*/
*++tmpPtr = -0.5992427934E-02;    /* ET(8,185)*/
*++tmpPtr = -0.4534262419E-01;    /* ET(9,185)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 186: */
 /* */
*++tmpPtr =  0.1389656305E+01;    /* ET(1,186)*/
*++tmpPtr =  0.1725424081E+00;    /* ET(2,186)*/
*++tmpPtr =  0.3295512497E+00;    /* ET(3,186)*/
*++tmpPtr = -0.1198873222E+00;    /* ET(4,186)*/
*++tmpPtr = -0.2289814800E+00;    /* ET(5,186)*/
*++tmpPtr = -0.2843078412E-01;    /* ET(6,186)*/
*++tmpPtr = -0.4827861488E+00;    /* ET(7,186)*/
*++tmpPtr = -0.7442720700E-02;    /* ET(8,186)*/
*++tmpPtr = -0.2715100534E-01;    /* ET(9,186)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 187: */
 /* */
*++tmpPtr =  0.1445291400E+01;    /* ET(1,187)*/
*++tmpPtr =  0.2386221290E+00;    /* ET(2,187)*/
*++tmpPtr =  0.2905831039E+00;    /* ET(3,187)*/
*++tmpPtr = -0.1724392623E+00;    /* ET(4,187)*/
*++tmpPtr = -0.2099886537E+00;    /* ET(5,187)*/
*++tmpPtr = -0.3466978297E-01;    /* ET(6,187)*/
*++tmpPtr = -0.5222167969E+00;    /* ET(7,187)*/
*++tmpPtr = -0.1423513051E-01;    /* ET(8,187)*/
*++tmpPtr = -0.2110963687E-01;    /* ET(9,187)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 188: */
 /* */
*++tmpPtr =  0.1483916163E+01;    /* ET(1,188)*/
*++tmpPtr =  0.2209532708E+00;    /* ET(2,188)*/
*++tmpPtr =  0.1864966452E+00;    /* ET(3,188)*/
*++tmpPtr = -0.1639380604E+00;    /* ET(4,188)*/
*++tmpPtr = -0.1383726746E+00;    /* ET(5,188)*/
*++tmpPtr = -0.2060352080E-01;    /* ET(6,188)*/
*++tmpPtr = -0.5505017638E+00;    /* ET(7,188)*/
*++tmpPtr = -0.1220508665E-01;    /* ET(8,188)*/
*++tmpPtr = -0.8695249446E-02;    /* ET(9,188)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 189: */
 /* */
*++tmpPtr =  0.1476989985E+01;    /* ET(1,189)*/
*++tmpPtr =  0.1422863454E+00;    /* ET(2,189)*/
*++tmpPtr =  0.3726066351E+00;    /* ET(3,189)*/
*++tmpPtr = -0.1050777510E+00;    /* ET(4,189)*/
*++tmpPtr = -0.2751681507E+00;    /* ET(5,189)*/
*++tmpPtr = -0.2650841884E-01;    /* ET(6,189)*/
*++tmpPtr = -0.5453748107E+00;    /* ET(7,189)*/
*++tmpPtr = -0.5061351229E-02;    /* ET(8,189)*/
*++tmpPtr = -0.3470892832E-01;    /* ET(9,189)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 190: */
 /* */
*++tmpPtr =  0.1462787390E+01;    /* ET(1,190)*/
*++tmpPtr =  0.1236966997E+00;    /* ET(2,190)*/
*++tmpPtr =  0.2217886895E+00;    /* ET(3,190)*/
*++tmpPtr = -0.9047098458E-01;    /* ET(4,190)*/
*++tmpPtr = -0.1622148454E+00;    /* ET(5,190)*/
*++tmpPtr = -0.1371726580E-01;    /* ET(6,190)*/
*++tmpPtr = -0.5349367261E+00;    /* ET(7,190)*/
*++tmpPtr = -0.3825218417E-02;    /* ET(8,190)*/
*++tmpPtr = -0.1229755580E-01;    /* ET(9,190)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 191: */
 /* */
*++tmpPtr =  0.1570778966E+01;    /* ET(1,191)*/
*++tmpPtr =  0.1144274473E+00;    /* ET(2,191)*/
*++tmpPtr =  0.2320267558E+00;    /* ET(3,191)*/
*++tmpPtr = -0.8987011760E-01;    /* ET(4,191)*/
*++tmpPtr = -0.1822313815E+00;    /* ET(5,191)*/
*++tmpPtr = -0.1327511482E-01;    /* ET(6,191)*/
*++tmpPtr = -0.6168366671E+00;    /* ET(7,191)*/
*++tmpPtr = -0.3273410257E-02;    /* ET(8,191)*/
*++tmpPtr = -0.1345910411E-01;    /* ET(9,191)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 192: */
 /* */
*++tmpPtr =  0.1534728646E+01;    /* ET(1,192)*/
*++tmpPtr =  0.1326557100E+00;    /* ET(2,192)*/
*++tmpPtr =  0.1219038740E+00;    /* ET(3,192)*/
*++tmpPtr = -0.1017952561E+00;    /* ET(4,192)*/
*++tmpPtr = -0.9354468435E-01;    /* ET(5,192)*/
*++tmpPtr = -0.8085622452E-02;    /* ET(6,192)*/
*++tmpPtr = -0.5888479948E+00;    /* ET(7,192)*/
*++tmpPtr = -0.4399383906E-02;    /* ET(8,192)*/
*++tmpPtr = -0.3715138417E-02;    /* ET(9,192)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 193: */
 /* */
*++tmpPtr =  0.9758982658E+00;    /* ET(1,193)*/
*++tmpPtr =  0.3342828751E+00;    /* ET(2,193)*/
*++tmpPtr =  0.9207419157E+00;    /* ET(3,193)*/
*++tmpPtr = -0.1631130278E+00;    /* ET(4,193)*/
*++tmpPtr = -0.4492751956E+00;    /* ET(5,193)*/
*++tmpPtr = -0.1538941264E+00;    /* ET(6,193)*/
*++tmpPtr = -0.2380943447E+00;    /* ET(7,193)*/
*++tmpPtr = -0.2793626115E-01;    /* ET(8,193)*/
*++tmpPtr = -0.2119414061E+00;    /* ET(9,193)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 194: */
 /* */
*++tmpPtr =  0.1223091960E+01;    /* ET(1,194)*/
*++tmpPtr =  0.3092411757E+00;    /* ET(2,194)*/
*++tmpPtr =  0.9734705687E+00;    /* ET(3,194)*/
*++tmpPtr = -0.1891151965E+00;    /* ET(4,194)*/
*++tmpPtr = -0.5953220129E+00;    /* ET(5,194)*/
*++tmpPtr = -0.1505185813E+00;    /* ET(6,194)*/
*++tmpPtr = -0.3739884794E+00;    /* ET(7,194)*/
*++tmpPtr = -0.2390752546E-01;    /* ET(8,194)*/
*++tmpPtr = -0.2369112372E+00;    /* ET(9,194)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 195: */
 /* */
*++tmpPtr =  0.1222711802E+01;    /* ET(1,195)*/
*++tmpPtr =  0.5540807247E+00;    /* ET(2,195)*/
*++tmpPtr =  0.8155465126E+00;    /* ET(3,195)*/
*++tmpPtr = -0.3387405276E+00;    /* ET(4,195)*/
*++tmpPtr = -0.4985891581E+00;    /* ET(5,195)*/
*++tmpPtr = -0.2259393036E+00;    /* ET(6,195)*/
*++tmpPtr = -0.3737560213E+00;    /* ET(7,195)*/
*++tmpPtr = -0.7675135881E-01;    /* ET(8,195)*/
*++tmpPtr = -0.1662790179E+00;    /* ET(9,195)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 196: */
 /* */
*++tmpPtr =  0.1444099426E+01;    /* ET(1,196)*/
*++tmpPtr =  0.5580272079E+00;    /* ET(2,196)*/
*++tmpPtr =  0.9505723715E+00;    /* ET(3,196)*/
*++tmpPtr = -0.4029233754E+00;    /* ET(4,196)*/
*++tmpPtr = -0.6863604784E+00;    /* ET(5,196)*/
*++tmpPtr = -0.2652226388E+00;    /* ET(6,196)*/
*++tmpPtr = -0.5213557482E+00;    /* ET(7,196)*/
*++tmpPtr = -0.7784859091E-01;    /* ET(8,196)*/
*++tmpPtr = -0.2258969694E+00;    /* ET(9,196)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 197: */
 /* */
*++tmpPtr =  0.1280543447E+01;    /* ET(1,197)*/
*++tmpPtr =  0.3755610287E+00;    /* ET(2,197)*/
*++tmpPtr =  0.6913205981E+00;    /* ET(3,197)*/
*++tmpPtr = -0.2404610962E+00;    /* ET(4,197)*/
*++tmpPtr = -0.4426330328E+00;    /* ET(5,197)*/
*++tmpPtr = -0.1298165321E+00;    /* ET(6,197)*/
*++tmpPtr = -0.4099478424E+00;    /* ET(7,197)*/
*++tmpPtr = -0.3526151925E-01;    /* ET(8,197)*/
*++tmpPtr = -0.1194810495E+00;    /* ET(9,197)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 198: */
 /* */
*++tmpPtr =  0.1372653484E+01;    /* ET(1,198)*/
*++tmpPtr =  0.4652582407E+00;    /* ET(2,198)*/
*++tmpPtr =  0.5585414767E+00;    /* ET(3,198)*/
*++tmpPtr = -0.3193191886E+00;    /* ET(4,198)*/
*++tmpPtr = -0.3833419383E+00;    /* ET(5,198)*/
*++tmpPtr = -0.1299330145E+00;    /* ET(6,198)*/
*++tmpPtr = -0.4710444212E+00;    /* ET(7,198)*/
*++tmpPtr = -0.5411630496E-01;    /* ET(8,198)*/
*++tmpPtr = -0.7799214125E-01;    /* ET(9,198)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 199: */
 /* */
*++tmpPtr =  0.1421039581E+01;    /* ET(1,199)*/
*++tmpPtr =  0.3477324843E+00;    /* ET(2,199)*/
*++tmpPtr =  0.5020613074E+00;    /* ET(3,199)*/
*++tmpPtr = -0.2470708042E+00;    /* ET(4,199)*/
*++tmpPtr = -0.3567245007E+00;    /* ET(5,199)*/
*++tmpPtr = -0.8729150891E-01;    /* ET(6,199)*/
*++tmpPtr = -0.5048383474E+00;    /* ET(7,199)*/
*++tmpPtr = -0.3022946790E-01;    /* ET(8,199)*/
*++tmpPtr = -0.6301639229E-01;    /* ET(9,199)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 200: */
 /* */
*++tmpPtr =  0.1454207778E+01;    /* ET(1,200)*/
*++tmpPtr =  0.3353322148E+00;    /* ET(2,200)*/
*++tmpPtr =  0.3620232642E+00;    /* ET(3,200)*/
*++tmpPtr = -0.2438213378E+00;    /* ET(4,200)*/
*++tmpPtr = -0.2632285058E+00;    /* ET(5,200)*/
*++tmpPtr = -0.6069903448E-01;    /* ET(6,200)*/
*++tmpPtr = -0.5286800265E+00;    /* ET(7,200)*/
*++tmpPtr = -0.2811192349E-01;    /* ET(8,200)*/
*++tmpPtr = -0.3276521340E-01;    /* ET(9,200)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 201: */
 /* */
*++tmpPtr =  0.1428423643E+01;    /* ET(1,201)*/
*++tmpPtr =  0.5399070382E+00;    /* ET(2,201)*/
*++tmpPtr =  0.6981824636E+00;    /* ET(3,201)*/
*++tmpPtr = -0.3856079876E+00;    /* ET(4,201)*/
*++tmpPtr = -0.4986501634E+00;    /* ET(5,201)*/
*++tmpPtr = -0.1884768158E+00;    /* ET(6,201)*/
*++tmpPtr = -0.5100985169E+00;    /* ET(7,201)*/
*++tmpPtr = -0.7287490368E-01;    /* ET(8,201)*/
*++tmpPtr = -0.1218646914E+00;    /* ET(9,201)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 202: */
 /* */
*++tmpPtr =  0.1611179352E+01;    /* ET(1,202)*/
*++tmpPtr =  0.4840326011E+00;    /* ET(2,202)*/
*++tmpPtr =  0.7096906304E+00;    /* ET(3,202)*/
*++tmpPtr = -0.3899316490E+00;    /* ET(4,202)*/
*++tmpPtr = -0.5717194676E+00;    /* ET(5,202)*/
*++tmpPtr = -0.1717566997E+00;    /* ET(6,202)*/
*++tmpPtr = -0.6489747167E+00;    /* ET(7,202)*/
*++tmpPtr = -0.5857188627E-01;    /* ET(8,202)*/
*++tmpPtr = -0.1259151995E+00;    /* ET(9,202)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 203: */
 /* */
*++tmpPtr =  0.1553122282E+01;    /* ET(1,203)*/
*++tmpPtr =  0.6005974412E+00;    /* ET(2,203)*/
*++tmpPtr =  0.5313029885E+00;    /* ET(3,203)*/
*++tmpPtr = -0.4664006233E+00;    /* ET(4,203)*/
*++tmpPtr = -0.4125892818E+00;    /* ET(5,203)*/
*++tmpPtr = -0.1595496088E+00;    /* ET(6,203)*/
*++tmpPtr = -0.6030471921E+00;    /* ET(7,203)*/
*++tmpPtr = -0.9017932415E-01;    /* ET(8,203)*/
*++tmpPtr = -0.7057072222E-01;    /* ET(9,203)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 204: */
 /* */
*++tmpPtr =  0.1700037956E+01;    /* ET(1,204)*/
*++tmpPtr =  0.5619463325E+00;    /* ET(2,204)*/
*++tmpPtr =  0.4026460648E+00;    /* ET(3,204)*/
*++tmpPtr = -0.4776650667E+00;    /* ET(4,204)*/
*++tmpPtr = -0.3422567844E+00;    /* ET(5,204)*/
*++tmpPtr = -0.1131327376E+00;    /* ET(6,204)*/
*++tmpPtr = -0.7225322723E+00;    /* ET(7,204)*/
*++tmpPtr = -0.7894591987E-01;    /* ET(8,204)*/
*++tmpPtr = -0.4053096101E-01;    /* ET(9,204)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 205: */
 /* */
*++tmpPtr =  0.1568797708E+01;    /* ET(1,205)*/
*++tmpPtr =  0.4065783024E+00;    /* ET(2,205)*/
*++tmpPtr =  0.4507254064E+00;    /* ET(3,205)*/
*++tmpPtr = -0.3189195395E+00;    /* ET(4,205)*/
*++tmpPtr = -0.3535484672E+00;    /* ET(5,205)*/
*++tmpPtr = -0.9162758291E-01;    /* ET(6,205)*/
*++tmpPtr = -0.6152815223E+00;    /* ET(7,205)*/
*++tmpPtr = -0.4132648185E-01;    /* ET(8,205)*/
*++tmpPtr = -0.5078834668E-01;    /* ET(9,205)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 206: */
 /* */
*++tmpPtr =  0.1600334406E+01;    /* ET(1,206)*/
*++tmpPtr =  0.4100409746E+00;    /* ET(2,206)*/
*++tmpPtr =  0.2593026161E+00;    /* ET(3,206)*/
*++tmpPtr = -0.3281013370E+00;    /* ET(4,206)*/
*++tmpPtr = -0.2074854374E+00;    /* ET(5,206)*/
*++tmpPtr = -0.5316234380E-01;    /* ET(6,206)*/
*++tmpPtr = -0.6402675509E+00;    /* ET(7,206)*/
*++tmpPtr = -0.4203340039E-01;    /* ET(8,206)*/
*++tmpPtr = -0.1680945978E-01;    /* ET(9,206)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 207: */
 /* */
*++tmpPtr =  0.1734088063E+01;    /* ET(1,207)*/
*++tmpPtr =  0.3839217424E+00;    /* ET(2,207)*/
*++tmpPtr =  0.4161036015E+00;    /* ET(3,207)*/
*++tmpPtr = -0.3328770399E+00;    /* ET(4,207)*/
*++tmpPtr = -0.3607801199E+00;    /* ET(5,207)*/
*++tmpPtr = -0.7987561077E-01;    /* ET(6,207)*/
*++tmpPtr = -0.7517653704E+00;    /* ET(7,207)*/
*++tmpPtr = -0.3684897721E-01;    /* ET(8,207)*/
*++tmpPtr = -0.4328554869E-01;    /* ET(9,207)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 208: */
 /* */
*++tmpPtr =  0.1822860360E+01;    /* ET(1,208)*/
*++tmpPtr =  0.3732457161E+00;    /* ET(2,208)*/
*++tmpPtr =  0.2783819735E+00;    /* ET(3,208)*/
*++tmpPtr = -0.3401874006E+00;    /* ET(4,208)*/
*++tmpPtr = -0.2537257075E+00;    /* ET(5,208)*/
*++tmpPtr = -0.5195244029E-01;    /* ET(6,208)*/
*++tmpPtr = -0.8307049274E+00;    /* ET(7,208)*/
*++tmpPtr = -0.3482809290E-01;    /* ET(8,208)*/
*++tmpPtr = -0.1937413029E-01;    /* ET(9,208)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 209: */
 /* */
*++tmpPtr =  0.1400086164E+01;    /* ET(1,209)*/
*++tmpPtr =  0.2213091105E+00;    /* ET(2,209)*/
*++tmpPtr =  0.6393244267E+00;    /* ET(3,209)*/
*++tmpPtr = -0.1549259126E+00;    /* ET(4,209)*/
*++tmpPtr = -0.4475546181E+00;    /* ET(5,209)*/
*++tmpPtr = -0.7074416429E-01;    /* ET(6,209)*/
*++tmpPtr = -0.4900602996E+00;    /* ET(7,209)*/
*++tmpPtr = -0.1224443130E-01;    /* ET(8,209)*/
*++tmpPtr = -0.1021839380E+00;    /* ET(9,209)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 210: */
 /* */
*++tmpPtr =  0.1533135295E+01;    /* ET(1,210)*/
*++tmpPtr =  0.1963017881E+00;    /* ET(2,210)*/
*++tmpPtr =  0.5504918098E+00;    /* ET(3,210)*/
*++tmpPtr = -0.1504786015E+00;    /* ET(4,210)*/
*++tmpPtr = -0.4219892323E+00;    /* ET(5,210)*/
*++tmpPtr = -0.5403126404E-01;    /* ET(6,210)*/
*++tmpPtr = -0.5876259208E+00;    /* ET(7,210)*/
*++tmpPtr = -0.9633597918E-02;    /* ET(8,210)*/
*++tmpPtr = -0.7576031238E-01;    /* ET(9,210)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 211: */
 /* */
*++tmpPtr =  0.1511827350E+01;    /* ET(1,211)*/
*++tmpPtr =  0.2613475025E+00;    /* ET(2,211)*/
*++tmpPtr =  0.4277670383E+00;    /* ET(3,211)*/
*++tmpPtr = -0.1975561529E+00;    /* ET(4,211)*/
*++tmpPtr = -0.3233549595E+00;    /* ET(5,211)*/
*++tmpPtr = -0.5589792505E-01;    /* ET(6,211)*/
*++tmpPtr = -0.5714054704E+00;    /* ET(7,211)*/
*++tmpPtr = -0.1707562804E-01;    /* ET(8,211)*/
*++tmpPtr = -0.4574616253E-01;    /* ET(9,211)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 212: */
 /* */
*++tmpPtr =  0.1577276468E+01;    /* ET(1,212)*/
*++tmpPtr =  0.2921322882E+00;    /* ET(2,212)*/
*++tmpPtr =  0.3169088364E+00;    /* ET(3,212)*/
*++tmpPtr = -0.2303866744E+00;    /* ET(4,212)*/
*++tmpPtr = -0.2499264479E+00;    /* ET(5,212)*/
*++tmpPtr = -0.4628964886E-01;    /* ET(6,212)*/
*++tmpPtr = -0.6219502687E+00;    /* ET(7,212)*/
*++tmpPtr = -0.2133531682E-01;    /* ET(8,212)*/
*++tmpPtr = -0.2510780469E-01;    /* ET(9,212)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 213: */
 /* */
*++tmpPtr =  0.1596556544E+01;    /* ET(1,213)*/
*++tmpPtr =  0.1855112314E+00;    /* ET(2,213)*/
*++tmpPtr =  0.3508694470E+00;    /* ET(3,213)*/
*++tmpPtr = -0.1480895728E+00;    /* ET(4,213)*/
*++tmpPtr = -0.2800914645E+00;    /* ET(5,213)*/
*++tmpPtr = -0.3254511207E-01;    /* ET(6,213)*/
*++tmpPtr = -0.6372481585E+00;    /* ET(7,213)*/
*++tmpPtr = -0.8603603579E-02;    /* ET(8,213)*/
*++tmpPtr = -0.3077734448E-01;    /* ET(9,213)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 214: */
 /* */
*++tmpPtr =  0.1624405146E+01;    /* ET(1,214)*/
*++tmpPtr =  0.2425359488E+00;    /* ET(2,214)*/
*++tmpPtr =  0.2066808492E+00;    /* ET(3,214)*/
*++tmpPtr = -0.1969883144E+00;    /* ET(4,214)*/
*++tmpPtr = -0.1678667068E+00;    /* ET(5,214)*/
*++tmpPtr = -0.2506376989E-01;    /* ET(6,214)*/
*++tmpPtr = -0.6596729755E+00;    /* ET(7,214)*/
*++tmpPtr = -0.1470592245E-01;    /* ET(8,214)*/
*++tmpPtr = -0.1067924406E-01;    /* ET(9,214)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 215: */
 /* */
*++tmpPtr =  0.1677300453E+01;    /* ET(1,215)*/
*++tmpPtr =  0.1384929717E+00;    /* ET(2,215)*/
*++tmpPtr =  0.2956604064E+00;    /* ET(3,215)*/
*++tmpPtr = -0.1161471531E+00;    /* ET(4,215)*/
*++tmpPtr = -0.2479556501E+00;    /* ET(5,215)*/
*++tmpPtr = -0.2047344297E-01;    /* ET(6,215)*/
*++tmpPtr = -0.7033341527E+00;    /* ET(7,215)*/
*++tmpPtr = -0.4795075394E-02;    /* ET(8,215)*/
*++tmpPtr = -0.2185376734E-01;    /* ET(9,215)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 216: */
 /* */
*++tmpPtr =  0.1711768150E+01;    /* ET(1,216)*/
*++tmpPtr =  0.1235954091E+00;    /* ET(2,216)*/
*++tmpPtr =  0.1589510441E+00;    /* ET(3,216)*/
*++tmpPtr = -0.1057833433E+00;    /* ET(4,216)*/
*++tmpPtr = -0.1360436529E+00;    /* ET(5,216)*/
*++tmpPtr = -0.9822809137E-02;    /* ET(6,216)*/
*++tmpPtr = -0.7325375080E+00;    /* ET(7,216)*/
*++tmpPtr = -0.3818956437E-02;    /* ET(8,216)*/
*++tmpPtr = -0.6316358689E-02;    /* ET(9,216)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 217: */
 /* */
*++tmpPtr =  0.1665512085E+01;    /* ET(1,217)*/
*++tmpPtr =  0.2676936686E+00;    /* ET(2,217)*/
*++tmpPtr =  0.5365031362E+00;    /* ET(3,217)*/
*++tmpPtr = -0.2229234874E+00;    /* ET(4,217)*/
*++tmpPtr = -0.4467762113E+00;    /* ET(5,217)*/
*++tmpPtr = -0.7180923969E-01;    /* ET(6,217)*/
*++tmpPtr = -0.6934825778E+00;    /* ET(7,217)*/
*++tmpPtr = -0.1791497320E-01;    /* ET(8,217)*/
*++tmpPtr = -0.7195889950E-01;    /* ET(9,217)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 218: */
 /* */
*++tmpPtr =  0.1748320580E+01;    /* ET(1,218)*/
*++tmpPtr =  0.2462860495E+00;    /* ET(2,218)*/
*++tmpPtr =  0.4256108105E+00;    /* ET(3,218)*/
*++tmpPtr = -0.2152934670E+00;    /* ET(4,218)*/
*++tmpPtr = -0.3720520437E+00;    /* ET(5,218)*/
*++tmpPtr = -0.5241100118E-01;    /* ET(6,218)*/
*++tmpPtr = -0.7641561627E+00;    /* ET(7,218)*/
*++tmpPtr = -0.1516420394E-01;    /* ET(8,218)*/
*++tmpPtr = -0.4528614134E-01;    /* ET(9,218)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 219: */
 /* */
*++tmpPtr =  0.1742892742E+01;    /* ET(1,219)*/
*++tmpPtr =  0.2442962229E+00;    /* ET(2,219)*/
*++tmpPtr =  0.2888238132E+00;    /* ET(3,219)*/
*++tmpPtr = -0.2128910571E+00;    /* ET(4,219)*/
*++tmpPtr = -0.2516944706E+00;    /* ET(5,219)*/
*++tmpPtr = -0.3527928516E-01;    /* ET(6,219)*/
*++tmpPtr = -0.7594187260E+00;    /* ET(7,219)*/
*++tmpPtr = -0.1492016111E-01;    /* ET(8,219)*/
*++tmpPtr = -0.2085479908E-01;    /* ET(9,219)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 220: */
 /* */
*++tmpPtr =  0.1827661514E+01;    /* ET(1,220)*/
*++tmpPtr =  0.2524173260E+00;    /* ET(2,220)*/
*++tmpPtr =  0.2041683048E+00;    /* ET(3,220)*/
*++tmpPtr = -0.2306667119E+00;    /* ET(4,220)*/
*++tmpPtr = -0.1865752786E+00;    /* ET(5,220)*/
*++tmpPtr = -0.2576780692E-01;    /* ET(6,220)*/
*++tmpPtr = -0.8350867033E+00;    /* ET(7,220)*/
*++tmpPtr = -0.1592862606E-01;    /* ET(8,220)*/
*++tmpPtr = -0.1042117365E-01;    /* ET(9,220)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 221: */
 /* */
*++tmpPtr =  0.1818982124E+01;    /* ET(1,221)*/
*++tmpPtr =  0.1496409327E+00;    /* ET(2,221)*/
*++tmpPtr =  0.3792175055E+00;    /* ET(3,221)*/
*++tmpPtr = -0.1360970885E+00;    /* ET(4,221)*/
*++tmpPtr = -0.3448949158E+00;    /* ET(5,221)*/
*++tmpPtr = -0.2837323211E-01;    /* ET(6,221)*/
*++tmpPtr = -0.8271739483E+00;    /* ET(7,221)*/
*++tmpPtr = -0.5598102696E-02;    /* ET(8,221)*/
*++tmpPtr = -0.3595148027E-01;    /* ET(9,221)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 222: */
 /* */
*++tmpPtr =  0.1890707970E+01;    /* ET(1,222)*/
*++tmpPtr =  0.1500784457E+00;    /* ET(2,222)*/
*++tmpPtr =  0.1993483901E+00;    /* ET(3,222)*/
*++tmpPtr = -0.1418772489E+00;    /* ET(4,222)*/
*++tmpPtr = -0.1884547919E+00;    /* ET(5,222)*/
*++tmpPtr = -0.1495894790E-01;    /* ET(6,222)*/
*++tmpPtr = -0.8936941624E+00;    /* ET(7,222)*/
*++tmpPtr = -0.5630884785E-02;    /* ET(8,222)*/
*++tmpPtr = -0.9934945032E-02;    /* ET(9,222)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 223: */
 /* */
*++tmpPtr =  0.2049684286E+01;    /* ET(1,223)*/
*++tmpPtr =  0.1898526996E+00;    /* ET(2,223)*/
*++tmpPtr =  0.3515543640E+00;    /* ET(3,223)*/
*++tmpPtr = -0.1945690364E+00;    /* ET(4,223)*/
*++tmpPtr = -0.3602877259E+00;    /* ET(5,223)*/
*++tmpPtr = -0.3337177262E-01;    /* ET(6,223)*/
*++tmpPtr = -0.1050301313E+01;    /* ET(7,223)*/
*++tmpPtr = -0.9011011571E-02;    /* ET(8,223)*/
*++tmpPtr = -0.3089761920E-01;    /* ET(9,223)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 224: */
 /* */
*++tmpPtr =  0.2145249844E+01;    /* ET(1,224)*/
*++tmpPtr =  0.1695440561E+00;    /* ET(2,224)*/
*++tmpPtr =  0.1919851154E+00;    /* ET(3,224)*/
*++tmpPtr = -0.1818571538E+00;    /* ET(4,224)*/
*++tmpPtr = -0.2059280127E+00;    /* ET(5,224)*/
*++tmpPtr = -0.1627496816E-01;    /* ET(6,224)*/
*++tmpPtr = -0.1150524139E+01;    /* ET(7,224)*/
*++tmpPtr = -0.7186296396E-02;    /* ET(8,224)*/
*++tmpPtr = -0.9214571677E-02;    /* ET(9,224)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 225: */
 /* */
*++tmpPtr =  0.8613638282E+00;    /* ET(1,225)*/
*++tmpPtr =  0.1473110199E+01;    /* ET(2,225)*/
*++tmpPtr =  0.1043792486E+01;    /* ET(3,225)*/
*++tmpPtr = -0.6344419718E+00;    /* ET(4,225)*/
*++tmpPtr = -0.4495425224E+00;    /* ET(5,225)*/
*++tmpPtr = -0.7688106894E+00;    /* ET(6,225)*/
*++tmpPtr = -0.1854869127E+00;    /* ET(7,225)*/
*++tmpPtr = -0.5425134301E+00;    /* ET(8,225)*/
*++tmpPtr = -0.2723756731E+00;    /* ET(9,225)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 226: */
 /* */
*++tmpPtr =  0.1167150497E+01;    /* ET(1,226)*/
*++tmpPtr =  0.1635484576E+01;    /* ET(2,226)*/
*++tmpPtr =  0.1497820497E+01;    /* ET(3,226)*/
*++tmpPtr = -0.9544282556E+00;    /* ET(4,226)*/
*++tmpPtr = -0.8740909100E+00;    /* ET(5,226)*/
*++tmpPtr = -0.1224831104E+01;    /* ET(6,226)*/
*++tmpPtr = -0.3405600488E+00;    /* ET(7,226)*/
*++tmpPtr = -0.6687024236E+00;    /* ET(8,226)*/
*++tmpPtr = -0.5608665943E+00;    /* ET(9,226)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 227: */
 /* */
*++tmpPtr =  0.1240597725E+01;    /* ET(1,227)*/
*++tmpPtr =  0.9323647022E+00;    /* ET(2,227)*/
*++tmpPtr =  0.1375733018E+01;    /* ET(3,227)*/
*++tmpPtr = -0.5783447623E+00;    /* ET(4,227)*/
*++tmpPtr = -0.8533656597E+00;    /* ET(5,227)*/
*++tmpPtr = -0.6413424015E+00;    /* ET(6,227)*/
*++tmpPtr = -0.3847706914E+00;    /* ET(7,227)*/
*++tmpPtr = -0.2173259854E+00;    /* ET(8,227)*/
*++tmpPtr = -0.4731603265E+00;    /* ET(9,227)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 228: */
 /* */
*++tmpPtr =  0.1569657087E+01;    /* ET(1,228)*/
*++tmpPtr =  0.1075022340E+01;    /* ET(2,228)*/
*++tmpPtr =  0.1558907032E+01;    /* ET(3,228)*/
*++tmpPtr = -0.8437082767E+00;    /* ET(4,228)*/
*++tmpPtr = -0.1223474860E+01;    /* ET(5,228)*/
*++tmpPtr = -0.8379299641E+00;    /* ET(6,228)*/
*++tmpPtr = -0.6159558892E+00;    /* ET(7,228)*/
*++tmpPtr = -0.2889182866E+00;    /* ET(8,228)*/
*++tmpPtr = -0.6075477600E+00;    /* ET(9,228)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 229: */
 /* */
*++tmpPtr =  0.1365463138E+01;    /* ET(1,229)*/
*++tmpPtr =  0.1278931975E+01;    /* ET(2,229)*/
*++tmpPtr =  0.7796534896E+00;    /* ET(3,229)*/
*++tmpPtr = -0.8731672764E+00;    /* ET(4,229)*/
*++tmpPtr = -0.5322940350E+00;    /* ET(5,229)*/
*++tmpPtr = -0.4985618889E+00;    /* ET(6,229)*/
*++tmpPtr = -0.4661224186E+00;    /* ET(7,229)*/
*++tmpPtr = -0.4089167416E+00;    /* ET(8,229)*/
*++tmpPtr = -0.1519648880E+00;    /* ET(9,229)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 230: */
 /* */
*++tmpPtr =  0.1805636048E+01;    /* ET(1,230)*/
*++tmpPtr =  0.1499251127E+01;    /* ET(2,230)*/
*++tmpPtr =  0.7207165360E+00;    /* ET(3,230)*/
*++tmpPtr = -0.1353550911E+01;    /* ET(4,230)*/
*++tmpPtr = -0.6506758332E+00;    /* ET(5,230)*/
*++tmpPtr = -0.5402675271E+00;    /* ET(6,230)*/
*++tmpPtr = -0.8150803447E+00;    /* ET(7,230)*/
*++tmpPtr = -0.5619385242E+00;    /* ET(8,230)*/
*++tmpPtr = -0.1298580766E+00;    /* ET(9,230)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 231: */
 /* */
*++tmpPtr =  0.1895483255E+01;    /* ET(1,231)*/
*++tmpPtr =  0.1318442702E+01;    /* ET(2,231)*/
*++tmpPtr =  0.1187757611E+01;    /* ET(3,231)*/
*++tmpPtr = -0.1249543071E+01;    /* ET(4,231)*/
*++tmpPtr = -0.1125687242E+01;    /* ET(5,231)*/
*++tmpPtr = -0.7829951644E+00;    /* ET(6,231)*/
*++tmpPtr = -0.8982142210E+00;    /* ET(7,231)*/
*++tmpPtr = -0.4345727861E+00;    /* ET(8,231)*/
*++tmpPtr = -0.3526920080E+00;    /* ET(9,231)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 232: */
 /* */
*++tmpPtr =  0.2469914913E+01;    /* ET(1,232)*/
*++tmpPtr =  0.1380718708E+01;    /* ET(2,232)*/
*++tmpPtr =  0.1336308599E+01;    /* ET(3,232)*/
*++tmpPtr = -0.1705128789E+01;    /* ET(4,232)*/
*++tmpPtr = -0.1650284290E+01;    /* ET(5,232)*/
*++tmpPtr = -0.9225330949E+00;    /* ET(6,232)*/
*++tmpPtr = -0.1525119901E+01;    /* ET(7,232)*/
*++tmpPtr = -0.4765960276E+00;    /* ET(8,232)*/
*++tmpPtr = -0.4464301467E+00;    /* ET(9,232)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 233: */
 /* */
*++tmpPtr =  0.1676719666E+01;    /* ET(1,233)*/
*++tmpPtr =  0.7492749095E+00;    /* ET(2,233)*/
*++tmpPtr =  0.1100798845E+01;    /* ET(3,233)*/
*++tmpPtr = -0.6281620264E+00;    /* ET(4,233)*/
*++tmpPtr = -0.9228655100E+00;    /* ET(5,233)*/
*++tmpPtr = -0.4124004543E+00;    /* ET(6,233)*/
*++tmpPtr = -0.7028472424E+00;    /* ET(7,233)*/
*++tmpPtr = -0.1403532177E+00;    /* ET(8,233)*/
*++tmpPtr = -0.3029395044E+00;    /* ET(9,233)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 234: */
 /* */
*++tmpPtr =  0.2061039925E+01;    /* ET(1,234)*/
*++tmpPtr =  0.7476313710E+00;    /* ET(2,234)*/
*++tmpPtr =  0.1372468710E+01;    /* ET(3,234)*/
*++tmpPtr = -0.7704489827E+00;    /* ET(4,234)*/
*++tmpPtr = -0.1414356232E+01;    /* ET(5,234)*/
*++tmpPtr = -0.5130503178E+00;    /* ET(6,234)*/
*++tmpPtr = -0.1061971307E+01;    /* ET(7,234)*/
*++tmpPtr = -0.1397381574E+00;    /* ET(8,234)*/
*++tmpPtr = -0.4709175527E+00;    /* ET(9,234)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 235: */
 /* */
*++tmpPtr =  0.2184015989E+01;    /* ET(1,235)*/
*++tmpPtr =  0.9024310708E+00;    /* ET(2,235)*/
*++tmpPtr =  0.9799290299E+00;    /* ET(3,235)*/
*++tmpPtr = -0.9854618907E+00;    /* ET(4,235)*/
*++tmpPtr = -0.1070090294E+01;    /* ET(5,235)*/
*++tmpPtr = -0.4421591759E+00;    /* ET(6,235)*/
*++tmpPtr = -0.1192481399E+01;    /* ET(7,235)*/
*++tmpPtr = -0.2035954595E+00;    /* ET(8,235)*/
*++tmpPtr = -0.2400652319E+00;    /* ET(9,235)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 236: */
 /* */
*++tmpPtr =  0.2416801453E+01;    /* ET(1,236)*/
*++tmpPtr =  0.1023732066E+01;    /* ET(2,236)*/
*++tmpPtr =  0.5968307853E+00;    /* ET(3,236)*/
*++tmpPtr = -0.1237078667E+01;    /* ET(4,236)*/
*++tmpPtr = -0.7212107182E+00;    /* ET(5,236)*/
*++tmpPtr = -0.3054974079E+00;    /* ET(6,236)*/
*++tmpPtr = -0.1460232377E+01;    /* ET(7,236)*/
*++tmpPtr = -0.2620068491E+00;    /* ET(8,236)*/
*++tmpPtr = -0.8905173838E-01;    /* ET(9,236)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 237: */
 /* */
*++tmpPtr =  0.1953039289E+01;    /* ET(1,237)*/
*++tmpPtr =  0.1841194034E+01;    /* ET(2,237)*/
*++tmpPtr =  0.1880902648E+01;    /* ET(3,237)*/
*++tmpPtr = -0.1797962070E+01;    /* ET(4,237)*/
*++tmpPtr = -0.1836738348E+01;    /* ET(5,237)*/
*++tmpPtr = -0.1731553316E+01;    /* ET(6,237)*/
*++tmpPtr = -0.9535905719E+00;    /* ET(7,237)*/
*++tmpPtr = -0.8474988341E+00;    /* ET(8,237)*/
*++tmpPtr = -0.8844487071E+00;    /* ET(9,237)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 238: */
 /* */
*++tmpPtr =  0.4274940968E+01;    /* ET(1,238)*/
*++tmpPtr =  0.3349903584E+01;    /* ET(2,238)*/
*++tmpPtr =  0.3654156685E+01;    /* ET(3,238)*/
*++tmpPtr = -0.7160320282E+01;    /* ET(4,238)*/
*++tmpPtr = -0.7810652733E+01;    /* ET(5,238)*/
*++tmpPtr = -0.6120536804E+01;    /* ET(6,238)*/
*++tmpPtr = -0.4568780422E+01;    /* ET(7,238)*/
*++tmpPtr = -0.2805463552E+01;    /* ET(8,238)*/
*++tmpPtr = -0.3338215351E+01;    /* ET(9,238)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 239: */
 /* */
*++tmpPtr =  0.2870590925E+01;    /* ET(1,239)*/
*++tmpPtr =  0.9051432014E+00;    /* ET(2,239)*/
*++tmpPtr =  0.1113256693E+01;    /* ET(3,239)*/
*++tmpPtr = -0.1299147844E+01;    /* ET(4,239)*/
*++tmpPtr = -0.1597852230E+01;    /* ET(5,239)*/
*++tmpPtr = -0.5038283467E+00;    /* ET(6,239)*/
*++tmpPtr = -0.2060072899E+01;    /* ET(7,239)*/
*++tmpPtr = -0.2048210502E+00;    /* ET(8,239)*/
*++tmpPtr = -0.3098351359E+00;    /* ET(9,239)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 240: */
 /* */
*++tmpPtr =  0.3778354883E+01;    /* ET(1,240)*/
*++tmpPtr =  0.1458825827E+01;    /* ET(2,240)*/
*++tmpPtr =  0.1076831579E+01;    /* ET(3,240)*/
*++tmpPtr = -0.2755980730E+01;    /* ET(4,240)*/
*++tmpPtr = -0.2034325838E+01;    /* ET(5,240)*/
*++tmpPtr = -0.7854548693E+00;    /* ET(6,240)*/
*++tmpPtr = -0.3568991184E+01;    /* ET(7,240)*/
*++tmpPtr = -0.5320432186E+00;    /* ET(8,240)*/
*++tmpPtr = -0.2898915708E+00;    /* ET(9,240)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 241: */
 /* */
*++tmpPtr =  0.1628960371E+01;    /* ET(1,241)*/
*++tmpPtr =  0.3091737926E+00;    /* ET(2,241)*/
*++tmpPtr =  0.8609257340E+00;    /* ET(3,241)*/
*++tmpPtr = -0.2518159449E+00;    /* ET(4,241)*/
*++tmpPtr = -0.7012069225E+00;    /* ET(5,241)*/
*++tmpPtr = -0.1330878288E+00;    /* ET(6,241)*/
*++tmpPtr = -0.6633780003E+00;    /* ET(7,241)*/
*++tmpPtr = -0.2389710955E-01;    /* ET(8,241)*/
*++tmpPtr = -0.1852982789E+00;    /* ET(9,241)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 242: */
 /* */
*++tmpPtr =  0.1838645458E+01;    /* ET(1,242)*/
*++tmpPtr =  0.3426188827E+00;    /* ET(2,242)*/
*++tmpPtr =  0.7104979753E+00;    /* ET(3,242)*/
*++tmpPtr = -0.3149772882E+00;    /* ET(4,242)*/
*++tmpPtr = -0.6531769037E+00;    /* ET(5,242)*/
*++tmpPtr = -0.1217150092E+00;    /* ET(6,242)*/
*++tmpPtr = -0.8451542258E+00;    /* ET(7,242)*/
*++tmpPtr = -0.2934692241E-01;    /* ET(8,242)*/
*++tmpPtr = -0.1262018532E+00;    /* ET(9,242)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 243: */
 /* */
*++tmpPtr =  0.1846755624E+01;    /* ET(1,243)*/
*++tmpPtr =  0.5515829325E+00;    /* ET(2,243)*/
*++tmpPtr =  0.6212229133E+00;    /* ET(3,243)*/
*++tmpPtr = -0.5093194246E+00;    /* ET(4,243)*/
*++tmpPtr = -0.5736234188E+00;    /* ET(5,243)*/
*++tmpPtr = -0.1713279784E+00;    /* ET(6,243)*/
*++tmpPtr = -0.8526265621E+00;    /* ET(7,243)*/
*++tmpPtr = -0.7606092840E-01;    /* ET(8,243)*/
*++tmpPtr = -0.9647947550E-01;    /* ET(9,243)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 244: */
 /* */
*++tmpPtr =  0.2000564098E+01;    /* ET(1,244)*/
*++tmpPtr =  0.5501123071E+00;    /* ET(2,244)*/
*++tmpPtr =  0.3876183033E+00;    /* ET(3,244)*/
*++tmpPtr = -0.5502674580E+00;    /* ET(4,244)*/
*++tmpPtr = -0.3877276182E+00;    /* ET(5,244)*/
*++tmpPtr = -0.1066167951E+00;    /* ET(6,244)*/
*++tmpPtr = -0.1000564218E+01;    /* ET(7,244)*/
*++tmpPtr = -0.7565588504E-01;    /* ET(8,244)*/
*++tmpPtr = -0.3756198660E-01;    /* ET(9,244)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 245: */
 /* */
*++tmpPtr =  0.1922148108E+01;    /* ET(1,245)*/
*++tmpPtr =  0.2716255784E+00;    /* ET(2,245)*/
*++tmpPtr =  0.5573853254E+00;    /* ET(3,245)*/
*++tmpPtr = -0.2610523105E+00;    /* ET(4,245)*/
*++tmpPtr = -0.5356885791E+00;    /* ET(5,245)*/
*++tmpPtr = -0.7570005953E-01;    /* ET(6,245)*/
*++tmpPtr = -0.9236633182E+00;    /* ET(7,245)*/
*++tmpPtr = -0.1844511554E-01;    /* ET(8,245)*/
*++tmpPtr = -0.7766959816E-01;    /* ET(9,245)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 246: */
 /* */
*++tmpPtr =  0.1922436237E+01;    /* ET(1,246)*/
*++tmpPtr =  0.2639802992E+00;    /* ET(2,246)*/
*++tmpPtr =  0.3617969155E+00;    /* ET(3,246)*/
*++tmpPtr = -0.2537426353E+00;    /* ET(4,246)*/
*++tmpPtr = -0.3477657735E+00;    /* ET(5,246)*/
*++tmpPtr = -0.4775362462E-01;    /* ET(6,246)*/
*++tmpPtr = -0.9239403009E+00;    /* ET(7,246)*/
*++tmpPtr = -0.1742139831E-01;    /* ET(8,246)*/
*++tmpPtr = -0.3272425383E-01;    /* ET(9,246)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 247: */
 /* */
*++tmpPtr =  0.2060506582E+01;    /* ET(1,247)*/
*++tmpPtr =  0.3731681705E+00;    /* ET(2,247)*/
*++tmpPtr =  0.4271655977E+00;    /* ET(3,247)*/
*++tmpPtr = -0.3844577074E+00;    /* ET(4,247)*/
*++tmpPtr = -0.4400887489E+00;    /* ET(5,247)*/
*++tmpPtr = -0.7970230281E-01;    /* ET(6,247)*/
*++tmpPtr = -0.1061421752E+01;    /* ET(7,247)*/
*++tmpPtr = -0.3481362015E-01;    /* ET(8,247)*/
*++tmpPtr = -0.4561761394E-01;    /* ET(9,247)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 248: */
 /* */
*++tmpPtr =  0.2071478844E+01;    /* ET(1,248)*/
*++tmpPtr =  0.3245634437E+00;    /* ET(2,248)*/
*++tmpPtr =  0.2477229089E+00;    /* ET(3,248)*/
*++tmpPtr = -0.3361631334E+00;    /* ET(4,248)*/
*++tmpPtr = -0.2565763593E+00;    /* ET(5,248)*/
*++tmpPtr = -0.4020089656E-01;    /* ET(6,248)*/
*++tmpPtr = -0.1072756052E+01;    /* ET(7,248)*/
*++tmpPtr = -0.2633535489E-01;    /* ET(8,248)*/
*++tmpPtr = -0.1534165908E-01;    /* ET(9,248)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 249: */
 /* */
*++tmpPtr =  0.2136144638E+01;    /* ET(1,249)*/
*++tmpPtr =  0.5034685731E+00;    /* ET(2,249)*/
*++tmpPtr =  0.1018387556E+01;    /* ET(3,249)*/
*++tmpPtr = -0.5377408266E+00;    /* ET(4,249)*/
*++tmpPtr = -0.1087711692E+01;    /* ET(5,249)*/
*++tmpPtr = -0.2563630641E+00;    /* ET(6,249)*/
*++tmpPtr = -0.1140778422E+01;    /* ET(7,249)*/
*++tmpPtr = -0.6337015331E-01;    /* ET(8,249)*/
*++tmpPtr = -0.2592783272E+00;    /* ET(9,249)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 250: */
 /* */
*++tmpPtr =  0.2166707754E+01;    /* ET(1,250)*/
*++tmpPtr =  0.4561027884E+00;    /* ET(2,250)*/
*++tmpPtr =  0.6564629078E+00;    /* ET(3,250)*/
*++tmpPtr = -0.4941208065E+00;    /* ET(4,250)*/
*++tmpPtr = -0.7111816406E+00;    /* ET(5,250)*/
*++tmpPtr = -0.1497072726E+00;    /* ET(6,250)*/
*++tmpPtr = -0.1173655748E+01;    /* ET(7,250)*/
*++tmpPtr = -0.5200744048E-01;    /* ET(8,250)*/
*++tmpPtr = -0.1077358797E+00;    /* ET(9,250)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 251: */
 /* */
*++tmpPtr =  0.2339734316E+01;    /* ET(1,251)*/
*++tmpPtr =  0.4615472853E+00;    /* ET(2,251)*/
*++tmpPtr =  0.5430480242E+00;    /* ET(3,251)*/
*++tmpPtr = -0.5399490595E+00;    /* ET(4,251)*/
*++tmpPtr = -0.6352941394E+00;    /* ET(5,251)*/
*++tmpPtr = -0.1253211647E+00;    /* ET(6,251)*/
*++tmpPtr = -0.1368589282E+01;    /* ET(7,251)*/
*++tmpPtr = -0.5325647444E-01;    /* ET(8,251)*/
*++tmpPtr = -0.7372529060E-01;    /* ET(9,251)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 252: */
 /* */
*++tmpPtr =  0.2451923847E+01;    /* ET(1,252)*/
*++tmpPtr =  0.5187888741E+00;    /* ET(2,252)*/
*++tmpPtr =  0.3581167758E+00;    /* ET(3,252)*/
*++tmpPtr = -0.6360154152E+00;    /* ET(4,252)*/
*++tmpPtr = -0.4390375316E+00;    /* ET(5,252)*/
*++tmpPtr = -0.9289350361E-01;    /* ET(6,252)*/
*++tmpPtr = -0.1502982616E+01;    /* ET(7,252)*/
*++tmpPtr = -0.6728547812E-01;    /* ET(8,252)*/
*++tmpPtr = -0.3206190467E-01;    /* ET(9,252)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 253: */
 /* */
*++tmpPtr =  0.2253475904E+01;    /* ET(1,253)*/
*++tmpPtr =  0.2490654588E+00;    /* ET(2,253)*/
*++tmpPtr =  0.4955404401E+00;    /* ET(3,253)*/
*++tmpPtr = -0.2806315124E+00;    /* ET(4,253)*/
*++tmpPtr = -0.5583442450E+00;    /* ET(5,253)*/
*++tmpPtr = -0.6171100214E-01;    /* ET(6,253)*/
*++tmpPtr = -0.1269538522E+01;    /* ET(7,253)*/
*++tmpPtr = -0.1550840028E-01;    /* ET(8,253)*/
*++tmpPtr = -0.6139008328E-01;    /* ET(9,253)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 254: */
 /* */
*++tmpPtr =  0.2435494661E+01;    /* ET(1,254)*/
*++tmpPtr =  0.2258067131E+00;    /* ET(2,254)*/
*++tmpPtr =  0.2905263007E+00;    /* ET(3,254)*/
*++tmpPtr = -0.2749755383E+00;    /* ET(4,254)*/
*++tmpPtr = -0.3537876308E+00;    /* ET(5,254)*/
*++tmpPtr = -0.3280139342E-01;    /* ET(6,254)*/
*++tmpPtr = -0.1482908487E+01;    /* ET(7,254)*/
*++tmpPtr = -0.1274716761E-01;    /* ET(8,254)*/
*++tmpPtr = -0.2110138349E-01;    /* ET(9,254)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 255: */
 /* */
*++tmpPtr =  0.2651094437E+01;    /* ET(1,255)*/
*++tmpPtr =  0.4440118670E+00;    /* ET(2,255)*/
*++tmpPtr =  0.5898106694E+00;    /* ET(3,255)*/
*++tmpPtr = -0.5885586739E+00;    /* ET(4,255)*/
*++tmpPtr = -0.7818218470E+00;    /* ET(5,255)*/
*++tmpPtr = -0.1309414655E+00;    /* ET(6,255)*/
*++tmpPtr = -0.1757075310E+01;    /* ET(7,255)*/
*++tmpPtr = -0.4928663373E-01;    /* ET(8,255)*/
*++tmpPtr = -0.8696915209E-01;    /* ET(9,255)*/
 /* */
 /* 9 ERROR TERMS OF GSP0 CENTROID # 256: */
 /* */
*++tmpPtr =  0.3183310986E+01;    /* ET(1,256)*/
*++tmpPtr =  0.3635759354E+00;    /* ET(2,256)*/
*++tmpPtr =  0.5308413506E+00;    /* ET(3,256)*/
*++tmpPtr = -0.5786876082E+00;    /* ET(4,256)*/
*++tmpPtr = -0.8449165225E+00;    /* ET(5,256)*/
*++tmpPtr = -0.9650056809E-01;    /* ET(6,256)*/
*++tmpPtr = -0.2533367157E+01;    /* ET(7,256)*/
*++tmpPtr = -0.3304686397E-01;    /* ET(8,256)*/
*++tmpPtr = -0.7044813037E-01;    /* ET(9,256)*/

