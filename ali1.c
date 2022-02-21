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
#define KOKO 20
#define AIKA 12
#include "ali1.h"
#include "ali2.h"


//kystään käyttäjältä haluttu lause(tiedoston/opetustialn nimi) ja tallennetaan vastaus, tämähän liitty tiedoston lukemiseen kun kysytään luettavan tiedoston nimi
void kysy(char lause[RIVI], char nimi[RIVI]){
	printf("%s",lause);
	scanf("%s", nimi);

}

//luetaan listaan tiedostosta tietoja ja tallennetaan ne niille tarkoitetulle tietueeseen
Solmu *lukeminen(Solmu *pAlku, char nimi[], char rivi[]) {
	
	//alustukset
	FILE *tiedosto;
	Solmu *ptrU;
	Solmu *pLoppu;
	int lkm=0;
	char *tila, *epoc, *paiva, *kk, *vuosi, *tunti, *min, *maara;
		
	//tämä se lisätty koodi pätkä joka tuli hoitaa palautukseen 2
	if (pAlku != NULL) {
		pAlku = tyhjennys(pAlku);
	}
	
	//kerrotaan minkä niminen tiedosto luetaan ja koitetaan avata se
	printf("Luetaan tiedosto '%s'\n", nimi);
	if ((tiedosto = fopen(nimi, "r")) == NULL){
		perror("Tiedoston lukeminen epäonnistui");
		exit(0);		
	}
	
	fgets(rivi, RIVI, tiedosto); //otsikko rivi pois
	
	//luetaan tiedostoa rivi riviltä läpi
	while (fgets(rivi, RIVI, tiedosto) != NULL){
		lkm ++;
		
		//erotetaan rivillä olevat arvot
		tila = strtok(rivi, ";");
		epoc = strtok(NULL,";");
		paiva = strtok(NULL,";");
		kk = strtok(NULL,";");
		vuosi = strtok(NULL,";");
		tunti = strtok(NULL,";");
		min = strtok(NULL,";");
		maara = strtok(NULL,"\n");
		
		//varataan tilaa
		if((ptrU = (Solmu *)malloc(sizeof(Solmu))) == NULL){
			perror("Muistin varaus epäonnistui");
			return NULL;
		}
		
		//tallennetaan arvot tietueeseen
		strcpy(ptrU->cTila,tila);
		ptrU->iEaika = atol(epoc);
		ptrU->iPaiva = atol(paiva);
		ptrU->iKk = atol(kk);
		ptrU->iVuosi = atol(vuosi);
		ptrU->iTunti = atol(tunti);
		ptrU->iMin = atol(min);	
		ptrU->iMaara = atol(maara);
		ptrU->pNext = NULL;
		
		
		//siirretään poitteria seuraavan kohtaan
		if (pAlku == NULL){
			pAlku = ptrU;
			pLoppu = ptrU;
		}else{
			pLoppu->pNext = ptrU;
			pLoppu = ptrU;
		}

	}
	
	//tiedoston sulkeminen ja kerrotaan monta riviä luettiin
	fclose(tiedosto);
	printf("Tiedosto '%s' luettu, %d riviä.\n",nimi,lkm);
	return pAlku;
}


//luetun tiedoston tietojen tallennus listasta tallennetaavaan tiedostoon
void tallennusKaikki(Solmu *pA, char nimi[], char tila[KOKO]){
	int lkm = 0;
	//alustukset
	FILE *tiedosto;
	Solmu *ptr;
	
	//koitetaana avata tiedostoa kirjoitusta varten
	if ((tiedosto = fopen(nimi, "w")) == NULL){
		perror("Tiedoston kirjoittaminen epäonnistui");
		exit(0);		
	}
	
	//poitteri on nyt lsitan ensimmäisessä alkiossa
	ptr = pA;
	
	//tiedoston kirjoitus
	fprintf(tiedosto, "Opetustila: %s\n",tila);
	fprintf(tiedosto, "Pvm Klo Lkm\n");
	while (ptr != NULL){
		if (strcmp(ptr->cTila,tila) == 0){
			lkm++;
			fprintf(tiedosto,"%d.%d.%d %02d:%02d %d\n",ptr->iPaiva,
			ptr->iKk,ptr->iVuosi,ptr->iTunti,ptr->iMin,ptr->iMaara);
		}
		ptr = ptr->pNext;
	}
	if (lkm != 0){
	//kerrotaan että tiedot tallennettu ja suljetaan tiedosto
	printf("Käyttödata tallennettu.\n");
	}else{
		printf("Ei ollut yhtään tietoa jota tallentaa\n");
	}
	fclose(tiedosto);
}
//tallennetaan tulokset tiedostoon
void tallennusTulos(tulos *pA, char nimi[]){
	//alustukset
	FILE *tiedosto;
	int j=0;
	tulos *ptr;
	
	//koitetaan avata tiedsotoa kirjoitusta varten
	if ((tiedosto = fopen(nimi, "w")) == NULL){
		perror("Tiedoston kirjoittaminen epäonnistui");
		exit(0);		
	}
	ptr = pA;
	
	//kirjoitetaan kellojen aika rivi
	fprintf(tiedosto, "Klo");
	for (int i=0;i<AIKA;i++){
		fprintf(tiedosto,";%02d:00-%02d:00",(i+7),(i+8));
	}
	fprintf(tiedosto,"\n");
	
	//kirjoitetaan tulos tiedot 
	while (ptr != NULL){
		fprintf(tiedosto,"%s %d.%d.%d",ptr->cTila,ptr->iPv,ptr->iKk,ptr->iVuosi);
		while(j<AIKA){
			fprintf(tiedosto,";%d",ptr->maksimit[j]);
			j++;
		}
		fprintf(tiedosto,"\n");
		j=0;
		ptr = ptr->pS;
	}
	
	//kerrtoaan että mihin tiedostoon tallennettu tiedot ja suljetaan tiedosto
	printf("Tiedot tallennettu tiedostoon: '%s'\n",nimi);
	fclose(tiedosto);
}

/*******************************************************************/
/* eof */
