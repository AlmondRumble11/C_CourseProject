/*******************************************************************/
/* CT60A2500 C-ohjelmoinnin perusteet
 * Otsikkotiedot:
 * Tekijä: Jesse Mustonen
 * Opiskelijanumero: 0541805
 * Päivämäärä:
 * Yhteistyö ja lähteet, nimi ja yhteistyön muoto:
 */
/*******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RIVI 100
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

//kaikki käytettyt aliohjelmat
void analyysi(Solmu *pA, char tila[KOKO]);
int valikko();
Solmu *tyhjennys(Solmu *pA);
void kysy(char lause[RIVI], char nimi[RIVI]);
Solmu *lukeminen(Solmu *pAlku, char nimi[], char rivi[]);
void tallennusKaikki(Solmu *pA, char nimi[], char tila[KOKO]);

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
	
	//keskiarvon laskeminen
	keskiarvo = summa / lkm;
	
	printf("Opetustilan %s käyttöanalyysi, %d alkiota:\n",tila,lkm);
	printf("  Avg  Min  Max\n");
	printf("%5d%5d%5d",keskiarvo,min,maks);
	printf("\n");
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

//luetaan listaan tiedostosta tietoja ja tallennetaan ne niille tarkoitetulle tietueeseen
Solmu *lukeminen(Solmu *pAlku, char nimi[], char rivi[]) {
	
	//alustukset
	FILE *tiedosto;
	Solmu *ptrU;
	Solmu *ptr;
	Solmu *pLoppu;
	int lkm=0;
	char *tila, *epoc, *paiva, *kk, *vuosi, *tunti, *min, *maara;
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
		//printf("%s ",p8);
		
		//siirretään poitteria seuraavan kohtaan
		if (pAlku == NULL){
			pAlku = ptrU;
			pLoppu = ptrU;
		}else{
			pLoppu->pNext = ptrU;
			pLoppu = ptrU;
			/*ptr = pAlku;
			while(ptr-> pNext != NULL){
				ptr = ptr->pNext;
			}
			ptr->pNext = ptrU;*/
		}
	}
	
	//tiedoston sulkeminen ja kerrotaan monta riviä luettiin
	fclose(tiedosto);
	printf("Tiedosto '%s' luettu, %d riviä.\n",nimi,lkm);
	return pAlku;
}

//luetun tiedoston tietojen tallennus listasta tallennetaavaan tiedostoon
void tallennusKaikki(Solmu *pA, char nimi[], char tila[KOKO]){

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
			fprintf(tiedosto,"%d.%d.%d %02d:%02d %d\n",ptr->iPaiva,
			ptr->iKk,ptr->iVuosi,ptr->iTunti,ptr->iMin,ptr->iMaara);
		}
		ptr = ptr->pNext;
	}
	
	//kerrotaan että tiedot tallennettu ja suljetaan tiedosto
	printf("Käyttödata tallennettu.\n");
	fclose(tiedosto);
}

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

//kystään käyttäjältä haluttu lause(tiedoston/opetustialn nimi) ja tallennetaan vastaus
void kysy(char lause[RIVI], char nimi[RIVI]){
	printf("%s",lause);
	scanf("%s", nimi);

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

//käyttöanalyysin suoritus luetuille arvoille ja analysoinnin tallennus omaan tietueeseen
tulos *kayttoanalyysi(Solmu *pA, tulos *pAlkuAna, char tila[KOKO]){
	//alsutukset
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
	tulos *pLoppu;
	//tulos *ptr2;
	//ptr2 = pAlkuAna;
	printf("Anna päivämäärä (päivä.kuukausi.vuosi): ");		
	scanf("%d.%d.%d", &pv,&kk,&vuosi);
	
	while (ptr1 != NULL){
		//printf("terve\n");
		
		//jos ei ole oikea opetustila niin seuraavan kierrokseen suoraan
		if ((strcmp(ptr1->cTila,tila) != 0)){
			ptr1 = ptr1->pNext;
			continue;
		}
	
		//printf("%s ja aika %d:%d\n",ptr1->cTila,ptr1->iPaiva,ptr1->iKk);
		//jos on oikea tila, mutta ei oikea pvm niin seuraavaan kierrokseen suoraan
		if ((ptr1->iVuosi != vuosi) || (ptr1->iKk != kk) || (ptr1->iPaiva != pv)){
			ptr1 = ptr1->pNext;
			continue;
		}
	
		//printf("%d:%d:%d ja aika %d:%d\n",ptr1->iPaiva,ptr1->iKk,ptr1->iVuosi,ptr1->iTunti,ptr1->iMin);
		
		//
		
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
			
			//printf("Luodaan tyhjälista\n");
			//Listan alustus nollilla
			for (int k = 0; i<AIKA;i++){
				ptrU->maksimit[k] = 0;
			}
			varmistus = 0;
		}
		//printf("edistystä %d:%d:%d ja aika %d:%d\n",ptr1->iPaiva,ptr1->iKk,ptr1->iVuosi,ptr1->iTunti,ptr1->iMin);
		//vain tunti välillä 7-19 tarkastellaan maksimeita
		if ((ptr1->iTunti < 19) && (ptr1->iTunti >= 7)) {
			ticket = 0;
			//printf("%d\n",ptr1->iMaara);
			//printf("ptr1->iTunti= %d ja tunnti = %d\n",ptr1->iTunti,kyseinentunti);
			//samat tunnit
			if (ptr1->iTunti == kyseinentunti){
				if (ptr1->iMaara >= maks){
					//printf("Maksimi jos ptr1>maks: %d\n",maks);
					maks = ptr1->iMaara;
				}else{
					//printf("%d\n", maks);
					maks = maks;
				}
			}else{
				kyseinentunti += 1;
				ptrU->maksimit[lkm] = maks;
				lkm += 1;
				//printf("maksimi on nyt %d\n",maks);
				maks = ptr1->iMaara;
			}
		}else{
			
		 	if (ticket == 1) { //estää ettei muut tunnit kuin 7-19 mene läpi eli ollaan suoritettu aikaisempi vertailu
		 		ptr1 = ptr1->pNext;
		 		continue;
		 	}
			//printf("coredumnp\n");
			
		 	//vika määrä joka tulee kun pompataan pois 
		 	ptrU->maksimit[lkm] = maks;
		 	
		 	//printf("ptr %d ja maks %d",ptrU->maksimit[lkm],maks);
		 	//printf("maksimi on nyt alla %d\n",maks);
		 	ticket = 0;
		 	//printf("päivä ennen laskua%d\n",pv);

			//siirretään pointteria
			/*if (pAlkuAna == NULL){
				pAlkuAna = ptrU;
				pLoppu = ptrU;
			}else{
				pLoppu->pS = ptrU;
				pLoppu = ptrU;
				*/
			if (pAlkuAna == NULL){
				pAlkuAna = ptrU;
			
			}else{
				//printf("jossain\n");
				tulos *ptr2 = pAlkuAna;
				//ptr2 = pAlkuAna;
				int p = 0;
				while(ptr2->pS != NULL){
					//printf("%d\n",p);
					ptr2 = ptr2->pS;
					p++;
				}
				//printf("Nyt ollaan täällä\n");
				ptr2->pS = ptrU;
			}
			//printf("Nyt ollaan täällä\n");
			varmistus = 1; //muutetaan taas varmistus yhdeksi jotta  voidan varata tilaa uusille arvoille
			lkm = 0; //nollataan listan indeksi 
			maks = 0; //nollataan maksimi
			kyseinentunti = 7; //aloitustunnin asetus uudestaan 7
			ticket = 1; //ticket yhdeski jotta ei päästä ylemmän ehdon jälkeiseen koodiin
			ticket2 = 1; //tarkistus että jotain ollaan tallennettuu uutten listaan
			
			pv = pv + 7; //hypättään viikko eteenpäin
			//päivän asetus jos menee kuukauden yli
			if ((pv > 31) && ((ptr1->iKk == 1) || (ptr1->iKk == 3) || (ptr1->iKk == 5) || (ptr1->iKk == 7) || (ptr1->iKk == 8) || (ptr1->iKk == 10) || (ptr1->iKk == 12))){
				if (kk > 12) {
					kk = kk - 12;
				}
				kk = kk + 1;
				pv = pv - 31;
			}else if ((pv > 28) && (ptr1->iKk == 2)) {
				if (kk > 12) {
					kk = kk - 12;
				}
				kk = kk + 1;
				pv = pv-28;
			}else if ((pv > 30) && ((ptr1->iKk == 4) || (ptr1->iKk == 6) || (ptr1->iKk == 9) || (ptr1->iKk == 11))){
				if (kk > 12) {
					kk = kk - 12;
				}
				kk = kk + 1;
				pv = pv - 30;
			}
			
			//printf("päivä laskun jälkeen %d\n",pv);
			//printf("\n\n");
		}
	ptr1 = ptr1->pNext;
	}
	//jos ei tallennettu mitään
	if ((pAlkuAna == NULL) || (ticket2 == 0)){ 
		printf("Ei analysoitavia alkioita\n");
	}else{
		printf("Aikavälianalyysi valmis.\n");
	}
	//printf("yess\n");
	return pAlkuAna;
}

//tulostetaan käyttäanalyysin tulos 
void tulostus(tulos *pA){
	tulos *ptr;
	ptr = pA;
	while (ptr != NULL){
		printf("Opetustilan %s käyttöanalyysi %d.%d.%d\n",ptr->cTila,ptr->iPv,ptr->iKk,ptr->iVuosi);
		printf("Klo:");
		for(int i=0;i<AIKA;i++){
			printf("%5d",(i+7));
		}
		printf("\n");
		printf("Lkm:");
		for(int j=0;j<AIKA;j++){
			printf("%5d",ptr->maksimit[j]);
		}
		printf("\n");
		/*printf("Klo:%5d%5d%5d%5d%5d%5d%5d%5d%5d%5d%5d%5d\n",7,8,9,10,11,12,13,14,15,16,17,18);
		printf("Lkm:%5d%5d%5d%5d%5d%5d%5d%5d%5d%5d%5d%5d\n",ptr->maksimit[0],ptr->maksimit[1],ptr->maksimit[2],ptr->maksimit[3],ptr->maksimit[4],ptr->maksimit[5],ptr->maksimit[6],ptr->maksimit[7],ptr->maksimit[8],ptr->maksimit[9],ptr->maksimit[10],ptr->maksimit[11]);*/
		ptr = ptr->pS;
	}
	
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
			//pAlku = tyhjennys(pAlku); 
			
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
