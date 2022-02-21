/*******************************************************************/
/* CT60A2500 C-ohjelmoinnin perusteet
 * Otsikkotiedot:
 * Tekijä: Jesse Mustonen
 * Opiskelijanumero: 0541805
 * Päivämäärä:24.3.2020
 * Yhteistyö ja lähteet, nimi ja yhteistyön muoto:luentotallenteet
 */
/*******************************************************************/
#ifndef ali2
#define ali2
#define KOKO 20
#define AIKA 12
//tietue lukemiseen ja luettujen tietojen tallennus tähän
struct Node {
	char cTila[KOKO];
	int iEaika;
	int iPaiva;
	int iKk;
	int iVuosi;
	int iMin;
	int iTunti;
	int iMaara;
	struct Node *pNext;
};
typedef struct Node Solmu;
//tietue tulosten tallennukseen kun analysoidaan 4-toiminnossa luettua tietoa
struct Tulos {
	char cTila[KOKO];
	int iVuosi;
	int iKk;
	int iPv;
	int maksimit[AIKA];
	struct Tulos *pS;
	
};
typedef struct Tulos tulos;
void analyysi(Solmu *pA, char tila[KOKO]);
Solmu *tyhjennys(Solmu *pA);
tulos *tyhjennysAna(tulos *pA);
tulos *kayttoanalyysi(Solmu *pA, tulos *pAlkuAna, char tila[KOKO]);
void tulostus(tulos *pA);
#endif
/*******************************************************************/
/* eof */
