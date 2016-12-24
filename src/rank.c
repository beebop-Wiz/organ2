#include <stdio.h>
#include <limits.h>
#include "rank.h"
#include <math.h>

void read_rank_func(struct rank_val *r, FILE *f) {
  fread(&r->bdef, 1, sizeof(int), f);
  fread(&r->v, 11, sizeof(float), f);
}

void read_harm_func(struct harm_val *h, FILE *f) {
  int i;
  for(i = 0; i < N_HARM; i++)
    read_rank_func(&h->v[i], f);
}

void r_read(struct rank *r, FILE *f) {
  char buf[64];
  fread(buf, 1, 32, f);
  r->fn = buf[30];
  r->fd = buf[31];
  fread(r->name, 1, 32, f);
  fread(buf, 1, 56, f);
  fread(r->mnem, 1, 8, f);
  fread(buf, 1, 56, f);
  fread(buf, 1, 8, f);
  read_rank_func(&r->vol, f);
  read_rank_func(&r->off, f);
  read_rank_func(&r->ran, f);
  read_rank_func(&r->ins, f);
  read_rank_func(&r->att, f);
  read_rank_func(&r->atd, f);
  read_rank_func(&r->dct, f);
  read_rank_func(&r->dcd, f);

  read_harm_func(&r->lev, f);
  read_harm_func(&r->h_ran, f);
  read_harm_func(&r->h_att, f);
  read_harm_func(&r->h_atp, f);

  int i, j;
  for(i = 0; i < N_NOTE; i++) {
    r->volcache[i] = powf(10, get_rv(r->vol, i) / 10);
    for(j = 0; j < N_HARM; j++) {
      r->hlcache[i][j] = powf(10, get_rv(r->lev.v[j], i) / 10);
    }
  }
}

#define N_SINE 1024

int sine_table[N_SINE] = {
  0, 13176152, 26351808, 39526473, 52699649, 65870842, 79039554, 92205291, 
105367557, 118525856, 131679694, 144828573, 157972001, 171109482, 184240521, 197364624, 
210481297, 223590046, 236690378, 249781799, 262863817, 275935939, 288997673, 302048528, 
315088011, 328115633, 341130902, 354133329, 367122425, 380097699, 393058664, 406004832, 
418935716, 431850828, 444749683, 457631794, 470496677, 483343848, 496172823, 508983119, 
521774253, 534545745, 547297113, 560027877, 572737559, 585425679, 598091759, 610735324, 
623355898, 635953004, 648526168, 661074919, 673598782, 686097287, 698569963, 711016340, 
723435950, 735828326, 748193000, 760529508, 772837385, 785116167, 797365393, 809584600, 
821773330, 833931124, 846057523, 858152071, 870214313, 882243794, 894240063, 906202666, 
918131155, 930025079, 941883992, 953707446, 965494996, 977246199, 988960613, 1000637796, 
1012277309, 1023878713, 1035441572, 1046965451, 1058449915, 1069894532, 1081298873, 1092662506, 
1103985005, 1115265942, 1126504895, 1137701438, 1148855152, 1159965615, 1171032410, 1182055120, 
1193033330, 1203966627, 1214854599, 1225696836, 1236492931, 1247242476, 1257945067, 1268600301, 
1279207777, 1289767096, 1300277860, 1310739674, 1321152143, 1331514875, 1341827482, 1352089573, 
1362300763, 1372460668, 1382568905, 1392625093, 1402628854, 1412579811, 1422477591, 1432321819, 
1442112125, 1451848142, 1461529502, 1471155841, 1480726796, 1490242008, 1499701117, 1509103769, 
1518449608, 1527738284, 1536969446, 1546142747, 1555257841, 1564314386, 1573312040, 1582250465, 
1591129324, 1599948283, 1608707010, 1617405175, 1626042451, 1634618513, 1643133037, 1651585704, 
1659976195, 1668304194, 1676569387, 1684771464, 1692910115, 1700985035, 1708995920, 1716942467, 
1724824377, 1732641354, 1740393104, 1748079335, 1755699757, 1763254084, 1770742031, 1778163316, 
1785517659, 1792804785, 1800024418, 1807176288, 1814260124, 1821275659, 1828222631, 1835100777, 
1841909838, 1848649559, 1855319684, 1861919964, 1868450150, 1874909995, 1881299257, 1887617695, 
1893865072, 1900041151, 1906145702, 1912178493, 1918139297, 1924027891, 1929844053, 1935587563, 
1941258206, 1946855767, 1952380037, 1957830807, 1963207872, 1968511030, 1973740080, 1978894827, 
1983975076, 1988980636, 1993911318, 1998766936, 2003547309, 2008252255, 2012881599, 2017435165, 
2021912782, 2026314282, 2030639499, 2034888269, 2039060434, 2043155836, 2047174321, 2051115737, 
2054979937, 2058766774, 2062476107, 2066107795, 2069661702, 2073137693, 2076535639, 2079855412, 
2083096885, 2086259938, 2089344451, 2092350308, 2095277396, 2098125604, 2100894826, 2103584958, 
2106195897, 2108727546, 2111179809, 2113552594, 2115845812, 2118059376, 2120193203, 2122247213, 
2124221329, 2126115475, 2127929581, 2129663579, 2131317403, 2132890991, 2134384283, 2135797224, 
2137129760, 2138381841, 2139553420, 2140644454, 2141654899, 2142584720, 2143433880, 2144202349, 
2144890096, 2145497096, 2146023326, 2146468766, 2146833400, 2147117214, 2147320196, 2147442341, 
2147483642, 2147444098, 2147323711, 2147122486, 2146840430, 2146477553, 2146033869, 2145509395, 
2144904151, 2144218159, 2143451446, 2142604039, 2141675972, 2140667278, 2139577997, 2138408168, 
2137157837, 2135827049, 2134415856, 2132924310, 2131352467, 2129700387, 2127968132, 2126155767, 
2124263360, 2122290982, 2120238708, 2118106615, 2115894784, 2113603297, 2111232240, 2108781704, 
2106251780, 2103642564, 2100954153, 2098186650, 2095340157, 2092414783, 2089410637, 2086327833, 
2083166487, 2079926717, 2076608645, 2073212398, 2069738101, 2066185887, 2062555888, 2058848242, 
2055063089, 2051200569, 2047260830, 2043244019, 2039150288, 2034979790, 2030732683, 2026409127, 
2022009284, 2017533320, 2012981403, 2008353705, 2003650400, 1998871666, 1994017681, 1989088629, 
1984084695, 1979006068, 1973852939, 1968625502, 1963323953, 1957948493, 1952499323, 1946976649, 
1941380679, 1935711623, 1929969694, 1924155110, 1918268088, 1912308851, 1906277623, 1900174630, 
1894000103, 1887754274, 1881437378, 1875049653, 1868591340, 1862062681, 1855463923, 1848795313, 
1842057103, 1835249546, 1828372900, 1821427421, 1814413374, 1807331020, 1800180627, 1792962464, 
1785676802, 1778323917, 1770904084, 1763417584, 1755864697, 1748245709, 1740560906, 1732810578, 
1724995015, 1717114513, 1709169368, 1701159880, 1693086349, 1684949079, 1676748378, 1668484554, 
1660157917, 1651768781, 1643317463, 1634804280, 1626229553, 1617593604, 1608896759, 1600139345, 
1591321692, 1582444132, 1573506998, 1564510628, 1555455360, 1546341535, 1537169496, 1527939589, 
1518652160, 1509307559, 1499906139, 1490448253, 1480934258, 1471364510, 1461739372, 1452059204, 
1442324372, 1432535242, 1422692182, 1412795563, 1402845758, 1392843140, 1382788088, 1372680979, 
1362522193, 1352312114, 1342051125, 1331739613, 1321377966, 1310966574, 1300505829, 1289996125, 
1279437858, 1268831424, 1258177224, 1247475658, 1236727130, 1225932043, 1215090804, 1204203822, 
1193271507, 1182294269, 1171272522, 1160206681, 1149097162, 1137944385, 1126748768, 1115510733, 
1104230704, 1092909104, 1081546360, 1070142901, 1058699154, 1047215552, 1035692525, 1024130509, 
1012529938, 1000891249, 989214880, 977501272, 965750864, 953964099, 942141420, 930283274, 
918390106, 906462364, 894500498, 882504956, 870476192, 858414658, 846320807, 834195096, 
822037980, 809849918, 797631368, 785382790, 773104645, 760797396, 748461506, 736097439, 
723705661, 711286637, 698840837, 686368728, 673870780, 661347463, 648799248, 636226609, 
623630019, 611009951, 598366881, 585701284, 573013638, 560304421, 547574109, 534823184, 
522052125, 509261413, 496451528, 483622954, 470776174, 457911671, 445029929, 432131433, 
419216669, 406286123, 393340282, 380379634, 367404665, 354415865, 341413723, 328398727, 
315371369, 302332138, 289281526, 276220023, 263148121, 250066313, 236975091, 223874948, 
210766376, 197649870, 184525923, 171395030, 158257684, 145114381, 131965614, 118811879, 
105653672, 92491487, 79325820, 66157167, 52986023, 39812884, 26638247, 13462607, 
286460, -12889697, -26065369, -39240060, -52413274, -65584515, -78753287, -91919094, 
-105081441, -118239831, -131393771, -144542764, -157686315, -170823930, -183955115, -197079374, 
-210196214, -223305140, -236405660, -249497281, -262579508, -275651851, -288713816, -301764912, 
-314804648, -327832533, -340848076, -353850788, -366840178, -379815758, -392777040, -405723534, 
-418654755, -431570216, -444469429, -457351909, -470217172, -483064734, -495894109, -508704816, 
-521496372, -534268296, -547020107, -559751324, -572461469, -585150063, -597816628, -610460687, 
-623081765, -635679387, -648253077, -660802363, -673326772, -685825834, -698299076, -710746030, 
-723166227, -735559200, -747924482, -760261607, -772570111, -784849530, -797099404, -809319269, 
-821508666, -833667137, -845794223, -857889469, -869952418, -881982616, -893979612, -905942952, 
-917872187, -929766868, -941626546, -953450776, -965239111, -976991110, -988706328, -1000384325, 
-1012024661, -1023626899, -1035190600, -1046715331, -1058200657, -1069646145, -1081051366, -1092415889, 
-1103739286, -1115021132, -1126261001, -1137458471, -1148613120, -1159724529, -1170792277, -1181815950, 
-1192795132, -1203729410, -1214618372, -1225461608, -1236258710, -1247009271, -1257712887, -1268369155, 
-1278977674, -1289538044, -1300049868, -1310512750, -1320926296, -1331290114, -1341603814, -1351867008, 
-1362079309, -1372240332, -1382349697, -1392407020, -1402411925, -1412364035, -1422262974, -1432108370, 
-1441899853, -1451637054, -1461319606, -1470947145, -1480519309, -1490035736, -1499496069, -1508899952, 
-1518247030, -1527536952, -1536769368, -1545943930, -1555060294, -1564118115, -1573117053, -1582056769, 
-1590936927, -1599757192, -1608517232, -1617216717, -1625855321, -1634432717, -1642948582, -1651402597, 
-1659794443, -1668123804, -1676390366, -1684593818, -1692733852, -1700810161, -1708822440, -1716770389, 
-1724653708, -1732472101, -1740225272, -1747912930, -1755534786, -1763090552, -1770579945, -1778002682, 
-1785358485, -1792647075, -1799868178, -1807021524, -1814106841, -1821123865, -1828072330, -1834951975, 
-1841762541, -1848503771, -1855175413, -1861777214, -1868308926, -1874770303, -1881161102, -1887481083, 
-1893730007, -1899907639, -1906013747, -1912048100, -1918010473, -1923900639, -1929718377, -1935463469, 
-1941135699, -1946734851, -1952260717, -1957713087, -1963091756, -1968396523, -1973627187, -1978783551, 
-1983865421, -1988872607, -1993804919, -1998662171, -2003444182, -2008150770, -2012781759, -2017336974, 
-2021816244, -2026219401, -2030546278, -2034796712, -2038970544, -2043067617, -2047087775, -2051030869, 
-2054896749, -2058685269, -2062396288, -2066029666, -2069585265, -2073062952, -2076462597, -2079784070, 
-2083027247, -2086192006, -2089278228, -2092285796, -2095214597, -2098064522, -2100835462, -2103527314, 
-2106139976, -2108673350, -2111127340, -2113501854, -2115796802, -2118012099, -2120147661, -2122203407, 
-2124179260, -2126075146, -2127890993, -2129626733, -2131282301, -2132857633, -2134352672, -2135767360, 
-2137101645, -2138355476, -2139528806, -2140621591, -2141633789, -2142565363, -2143416277, -2144186500, 
-2144876002, -2145484758, -2146012744, -2146459941, -2146826332, -2147111903, -2147316643, -2147440545, 
-2147483604, -2147445818, -2147327188, -2147127720, -2146847421, -2146486301, -2146044374, -2145521657, 
-2144918168, -2144233932, -2143468973, -2142623320, -2141697006, -2140690065, -2139602535, -2138434458, 
-2137185876, -2135856837, -2134447391, -2132957591, -2131387494, -2129737157, -2128006644, -2126196020, 
-2124305352, -2122334713, -2120284175, -2118153817, -2115943718, -2113653962, -2111284634, -2108835825, 
-2106307626, -2103700132, -2101013443, -2098247657, -2095402881, -2092479221, -2089476786, -2086395691, 
-2083236051, -2079997985, -2076681614, -2073287065, -2069814464, -2066263942, -2062635633, -2058929674, 
-2055146204, -2051285365, -2047347303, -2043332166, -2039240105, -2035071275, -2030825832, -2026503936, 
-2022105750, -2017631439, -2013081171, -2008455119, -2003753456, -1998976360, -1994124009, -1989196587, 
-1984194279, -1979117274, -1973965763, -1968739939, -1963439999, -1958066144, -1952618574, -1947097496, 
-1941503117, -1935835648, -1930095301, -1924282294, -1918396845, -1912439176, -1906409510, -1900308075, 
-1894135101, -1887890819, -1881575466, -1875189279, -1868732497, -1862205365, -1855608128, -1848941034, 
-1842204335, -1835398283, -1828523136, -1821579151, -1814566591, -1807485720, -1800336803, -1793120110, 
-1785835914, -1778484487, -1771066107, -1763581053, -1756029606, -1748412052, -1740728677, -1732979770, 
-1725165623, -1717286529, -1709342787, -1701334694, -1693262552, -1685126665, -1676927340, -1668664884, 
-1660339610, -1651951830, -1643501860, -1634990018, -1626416626, -1617782005, -1609086480, -1600330379, 
-1591514032, -1582637771, -1573701929, -1564706843, -1555652852, -1546540297, -1537369519, -1528140866, 
-1518854684, -1509511323, -1500111135, -1490654472, -1481141693, -1471573154, -1461949215, -1452270240, 
-1442536593, -1432748639, -1422906748, -1413011289, -1403062636, -1393061163, -1383007247, -1372901265, 
-1362743599, -1352534631, -1342274745, -1331964327, -1321603766, -1311193451, -1300733775, -1290225132, 
-1279667916, -1269062525, -1258409359, -1247708819, -1236961307, -1226167228, -1215326988, -1204440997, 
-1193509662, -1182533396, -1171512612, -1160447726, -1149339152, -1138187311, -1126992621, -1115755504, 
-1104476383, -1093155683, -1081793829, -1070391250, -1058948375, -1047465634, -1035943460, -1024382287, 
-1012782549, -1001144685, -989469131, -977756327, -966006714, -954220735, -942398832, -930541453, 
-918649041, -906722046, -894760917, -882766103, -870738056, -858677229, -846584076, -834459053, 
-822302615, -810115221, -797897328, -785649399, -773371892, -761065271, -748729998, -736366539, 
-723975358, -711556922, -699111699, -686640157, -674142765, -661619995, -649072317, -636500204, 
-623904129, -611284566, -598641991, -585976879, -573289708, -560580954, -547851096, -535100614, 
-522329988, -509539697, -496730225, -483902052, -471055662, -458191539, -445310167, -432412030, 
-419497615, -406567407, -393621893, -380661561, -367686899, -354698394, -341696537, -328681816, 
-315654721, -302615743, -289565373, -276504101, -263432420, -250350823, -237259800, -224159845, 
-211051452, -197935113, -184811323, -171680575, -158543364, -145400185, -132251532, -119097900, 
-105939784, -92777681, -79612084, -66443491, -53272396, -40099295, -26924685, -13749062, 
};

int sine_lookup(unsigned int theta) {
  int i = theta >> 22;
  return sine_table[i];
}

float get_rv(struct rank_val r, int note) {
  int i = note / 6;
  int k = note - 6 * i;
  float v = r.v[i];
  if(k) v += k * (r.v[i + 1] - v) / 6;
  return v;
}

float fundamental(float note) {
  return 440.0 * pow(2, (note - 33) / 12.0);
}

float r_advance(struct rank *r, int i) {
  int v = 0;
  float f0 = (fundamental(i) * r->fn / r->fd) * 2 / SAMPLERATE;
  int j;
  float vol = r->v_env[i] * r->volcache[i] ;
  for(j = 0; j < N_HARM; j++) {
    v += sine_lookup(r->h_phase[i][j]) * vol * r->hlcache[i][j];
    r->h_phase[i][j] += INT_MAX * f0 * (j + 1);
  }
  return ((float) v / (float) INT_MAX) * 10;
}
