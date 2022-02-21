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
#include "ali2.h"

//analysoidaan kyseisen tilan minini,keskiarvo ja maksimi käyttäjämäärä
void analyysi(Solmu *pA, char tila[KOKO]){
	//alustukset
	int lkm=0;
	int keskiarvo = 0;
	int min = 0;
	int maks = 0;
	int summa = 0;
	Solmu *ptr;
	ptr = pA;
	
	//käydään listaa läpi
	while (ptr != NULL){
		if (strcmp(ptr->cTila,tila) == 0){
			lkm ++;
			//minimin etsiminen
			if (min > ptr->iMaara){ 
				min = ptr->iMaara;
			}
			//maksimin etsiminen
			if (maks < ptr->iMaara){
				maks = ptr->iMaara;
			}
			//lasketaan summaa, joka sitten jaetaan keskiarvoa laskettaessa lukumäärällä (lkm)
			summa = summa + ptr->iMaara;
		}
		ptr = ptr->pNext;
	}
	if (lkm != 0){
	//keskiarvon laskeminen
	keskiarvo = summa / lkm;
	
	printf("Opetustilan %s käyttöanalyysi, %d alkiota:\n",tila,lkm);
	printf("  Avg  Min  Max\n");
	printf("%5d%5d%5d",keskiarvo,min,maks);
	printf("\n");
	}else{
		printf("Ei löytynyt yhtään dataa.\n");
	}
}

//luetun listan tyhjennys
Solmu *tyhjennys(Solmu *pA){
	Solmu *ptr = pA;
	while (ptr != NULL){
		pA = ptr-> pNext;
		free(ptr);
		ptr = pA;
	}
	return pA;
}

//tulos lisatan tyhjennys
tulos *tyhjennysAna(tulos *pA){
	tulos *ptr = pA;
	while (ptr != NULL){
		pA = ptr-> pS;
		free(ptr);
		ptr = pA;
	}
	return pA;
}
//käyttöanalyysin suoritus luetuille arvoille ja analysoinnin tallennus omaan tietueeseen
tulos *kayttoanalyysi(Solmu *pA, tulos *pAlkuAna, char tila[KOKO]){
	//alustukset
	Solmu *ptr1 = pA;
	int pv,kk,vuosi;
	int lkm=0;
	int maks=0;
	int i = 0;
	int kyseinentunti = 7; //aloitus tunti, joka nollataan aina kun ollaan tarkatettu 7-19 väli
	int varmistus = 1;
	int ticket = 1; //varmistaa että ponpataan aina viikko kerralla ennen kuin muutentaan päivämäärä ja siirretään poitteria lsitan seuravaan alkioon
	int ticket2 = 0; //varmistetaan jos yritetään analysoida tietoa jota ei ole jolloin ei lisätä listaan mitään
	tulos *ptrU;
	printf("Anna päivämäärä (päivä.kuukausi.vuosi): ");		
	scanf("%d.%d.%d", &pv,&kk,&vuosi);
	
	while (ptr1 != NULL){

		
		//jos ei ole oikea opetustila niin seuraavan kierrokseen suoraan
		if ((strcmp(ptr1->cTila,tila) != 0)){
			ptr1 = ptr1->pNext;
			continue;
		}
	
		//jos on oikea tila, mutta ei oikea pvm niin seuraavaan kierrokseen suoraan
		if ((ptr1->iVuosi != vuosi) || (ptr1->iKk != kk) || (ptr1->iPaiva != pv)){
			ptr1 = ptr1->pNext;
			continue;
		}
		
		/*varataan tila ja asetetaan arvot ja alustetaan kokonaisluku taulukko maksimeille nolliksi jolloin voidaan asettaa arvoja nollien tilalle
		varmistaa että varataan tilaa vasta kun ollaan siirretty poitteria eteen päin*/
		if (varmistus == 1){ 
			
			//varataan tilaa
			if((ptrU = (tulos *)malloc(sizeof(tulos))) == NULL){
				perror("Muistin varaus epäonnistui");
				return NULL;
			}
			
			//tallennetaan tietueseen arvot
			strcpy(ptrU->cTila,ptr1->cTila);
			ptrU->iVuosi = ptr1->iVuosi;
			ptrU->iKk = ptr1->iKk;
			ptrU->iPv = ptr1->iPaiva;		
			ptrU->pS = NULL;
			
			//Listan alustus nollilla
			for (int k = 0; i<AIKA;i++){
				ptrU->maksimit[k] = 0;
			}
			varmistus = 0;
		}
		
		//vain tunti välillä 7-19 tarkastellaan maksimeita
		if ((ptr1->iTunti < 19) && (ptr1->iTunti >= 7)) {
			ticket = 0;
			
			//samat tunnit
			if (ptr1->iTunti == kyseinentunti){
				if (ptr1->iMaara >= maks){
					maks = ptr1->iMaara;
				}else{
					maks = maks;
				}
			}else{
				kyseinentunti += 1;
				ptrU->maksimit[lkm] = maks;
				lkm += 1;
				maks = ptr1->iMaara;
			}
		}else{
			
		 	if (ticket == 1) { //estää ettei muut tunnit kuin 7-19 mene läpi eli ollaan suoritettu aikaisempi vertailu
		 		ptr1 = ptr1->pNext;
		 		continue;
		 	}
			
			
		 	//vika määrä joka tulee kun pompataan pois 
		 	ptrU->maksimit[lkm] = maks;
		 	

			//siirretään pointteria
	
			if (pAlkuAna == NULL){
				pAlkuAna = ptrU;
			}else{
				tulos *ptr2 = pAlkuAna;
				while(ptr2->pS != NULL){
					ptr2 = ptr2->pS;
				}
				ptr2->pS = ptrU;
			}
			//muutetaan taas varmistus yhdeksi jotta  voidan varata tilaa uusille arvoille
			varmistus = 1; 
			//nollataan listan indeksi  ja maksimi
			lkm = 0; 
			maks = 0; 
			//aloitustunnin asetus uudestaan 7
			kyseinentunti = 7;
			//ticket yhdeski jotta ei päästä ylemmän ehdon jälkeiseen koodiin
			ticket = 1; 
			//tarkistus että jotain ollaan tallennettuu uutten listaan
			ticket2 = 1; 
			//hypättään viikko eteenpäin
			pv = pv + 7; 
			//päivän asetus jos menee kuukauden yli
			if ((pv > 31) && ((ptr1->iKk == 1) || (ptr1->iKk == 3) || (ptr1->iKk == 5) || (ptr1->iKk == 7) || (ptr1->iKk == 8) || (ptr1->iKk == 10) || (ptr1->iKk == 12))){
				kk = kk + 1;
				pv = pv - 31;				
				if (kk > 12) { //jos mennään vuoden yli
					kk = kk - 12;
					vuosi = vuosi + 1;
				}
				
			}else if ((pv > 28) && (ptr1->iKk == 2)) {
				kk = kk + 1;
				pv = pv - 28;
				if (kk > 12) {//jos mennään vuoden yli
					kk = kk - 12;
					vuosi = vuosi + 1;
				}
				
			}else if ((pv > 30) && ((ptr1->iKk == 4) || (ptr1->iKk == 6) || (ptr1->iKk == 9) || (ptr1->iKk == 11))){
				kk = kk + 1;
				pv = pv - 30;
				if (kk > 12) {//jos mennään vuoden yli
					kk = kk - 12;
					vuosi = vuosi + 1;
				}
				
			}
		}
	ptr1 = ptr1->pNext;
	}
	//jos ei tallennettu mitään
	if ((pAlkuAna == NULL) || (ticket2 == 0)){ 
		printf("Ei analysoitavia alkioita\n");
	}else{
		printf("Aikavälianalyysi valmis.\n");
	}
	
	return pAlkuAna;
}
//tulostetaan käyttäanalyysin tulos 
void tulostus(tulos *pA){
	tulos *ptr;
	ptr = pA;
	while (ptr != NULL){
		printf("Opetustilan %s käyttöanalyysi %d.%d.%d\n",ptr->cTila,ptr->iPv,ptr->iKk,ptr->iVuosi);
		printf("Klo:");
		for(int i=0;i<AIKA;i++){ //eka ajat tulostetaan
			printf("%5d",(i+7));
		}
		printf("\n");
		printf("Lkm:");
		for(int j=0;j<AIKA;j++){//sitten maksimimäärät
			printf("%5d",ptr->maksimit[j]);
		}
		printf("\n");
		ptr = ptr->pS;
	}
	
}
/*******************************************************************/
/* eof */
