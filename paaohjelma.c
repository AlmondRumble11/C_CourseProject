/*******************************************************************/
/* CT60A2500 C-ohjelmoinnin perusteet
 * Otsikkotiedot:
 * Tekijä: Jesse Mustonen
 * Opiskelijanumero: 0541805
 * Päivämäärä:24.3.2020
 * Yhteistyö ja lähteet, nimi ja yhteistyön muoto:luentotallenteet
 */
/*******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RIVI 100
#include "ali1.h"
#include "ali2.h"


//valikko josta käyttäjä valikoi haluamansa toiminnon
int valikko(){
	int valinta;
	printf("\nValitse haluamasi toiminto alla olevasta valikosta:\n");
	printf("1) Lue tilan käyttö -tiedosto\n");
	printf("2) Tallenna listan tiedot\n");
	printf("3) Analysoi tiedot\n");
	printf("4) Suorita käyttöanalyysi opetustilalle\n");
	printf("5) Tulosta kaikki tulokset\n");
	printf("6) Tallenna tulokset tiedostoon\n");
	printf("7) Tyhjennä analyysilista\n");
	printf("0) Lopeta\n");
	printf("Valintasi: ");
	scanf("%d", &valinta);
	return valinta;
}
//pääohjelma
int main (void) {
	//alku asetukset
	Solmu *pAlku = NULL;
	tulos *pAlkuAnalyysi = NULL;
	int valinta = -1;
	char nimi[RIVI];
	char lause[RIVI];
	char rivi[RIVI];
	char tallennusTiedosto[] = "tulostiedot.txt";
	char tallennusAnalyysi[] = "tulostiedot.csv";
	
	printf("Tämä ohjelma analysoi tilan käyttö -tiedostoja.\n");
	do{
		valinta = valikko(); //otetaan valikosta käyttäjän antama valinta
		switch(valinta){
		case 1:
			//kopioidaan lause ja kysytään se käyttäjältä 
			strcpy(lause,"Anna luettavan tiedoston nimi: ");
			//lasueen vastaus on luettävan tiedoston nimi 
			kysy(lause,nimi);
			//luetaan tiedosto ja tallennetaan linkiettyyn liestaan tiedot sieltä
			pAlku = lukeminen(pAlku, nimi, rivi);
			break;
		case 2:
			//pitää olla luettu tiedot
			if (pAlku == NULL){
				printf("Ei tallennettavaa, lue ensin käyttötiedosto.\n");
			}else{
				//kopioidaan lause ja kysytään se käyttäjältä 
				strcpy(lause,"Anna opetustilan nimi: ");
				//lauseen vastaus on haluttu opetustila
				kysy(lause, nimi);
				//tietojen tallennus
				tallennusKaikki(pAlku, tallennusTiedosto, nimi);
			}
			break;
		case 3:
			//pitää olla luettu tiedot
			if (pAlku == NULL){
				printf("Ei analysoitavaa, lue ensin käyttötiedosto.\n");
			}else{
				//kopioidaan lause ja kysytään se käyttäjältä 
				strcpy(lause,"Anna opetustilan nimi: ");
				//lauseen vastaus on haluttu opetustila
				kysy(lause, nimi);
				//tehdään haluttu analyysi tiedoille halutussa opetustilassa
				analyysi(pAlku, nimi);
			}
			break;
		case 4:
			//pitää olla luettu tiedot
			if (pAlku == NULL){
				printf("Ei analysoitavaa, lue ensin käyttötiedosto.\n");
			}else{
				//kopioidaan lause ja kysytään se käyttäjältä 
				strcpy(lause,"Anna opetustilan nimi: ");
				//lauseen vastaus on haluttu opetustila
				kysy(lause, nimi);
				//tehdään haluttu käyttöanalyysi tiedoille halutussa opetustilassa
				pAlkuAnalyysi = kayttoanalyysi(pAlku, pAlkuAnalyysi, nimi);
				
			}
			break;
		case 5:
			//pitää olla analysoitu tiedot(eli case 4 tehty)
			if (pAlkuAnalyysi == NULL){
				printf("Tuloslista on tyhjä.\n");
			}else{
				//analyysin tulostus
				tulostus(pAlkuAnalyysi);
			}
			break;
		case 6:
			//pitää olla analysoitu tiedot(eli case 4 tehty)
			if (pAlkuAnalyysi == NULL){
				printf("Tuloslista on tyhjä.\n");
			}else{
				//tallennetaan käyttöanalyysin tulos tiedostoon
				tallennusTulos(pAlkuAnalyysi,tallennusAnalyysi);
			}
			break;
		case 7:
			//tuloslistan tyhjennys
			pAlkuAnalyysi = tyhjennysAna(pAlkuAnalyysi);
			printf("Tuloslista tyhjennetty.\n");
			break;
		case 0:
			//tyhjennetään molemmat linkitetyt listat ja kiitetään käyttäjää
			pAlku = tyhjennys(pAlku); 
			pAlkuAnalyysi = tyhjennysAna(pAlkuAnalyysi);
			printf("Kiitos ohjelman käytöstä.\n");
			printf("\n");
			break;
		default:
			//jos väärä valinta 
			printf("Tuntematon valinta, yritä uudestaan.\n");
			break;
		}
	} while(valinta != 0); //loopia niin pitkään ennen kuin käyttäjä antaa 0 valinnaksi
	return (0);
}
/*******************************************************************/
/* eof */
