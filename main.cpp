#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<cstdlib>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define SZEROKOSC_EKRANU 800
#define WYSOKOSC_EKRANU	600
#define TAK 1
#define NIE 0
#define MINELO 0
#define BRAK 0
#define NIE_PRZESUWAJ 0
#define LEWO 1
#define PRAWO 2
#define SRODEK 3
#define PELNOEKRANOWY 1
#define OKIENKOWY 0
#define NA_LEWO 0
#define NA_SRODEK 1
#define NA_PRAWO 2
#define LEWA_GRANICA_DROGI -240
#define PRAWA_GRANICA_DROGI 240
#define LEWE_POBOCZE -270
#define PRAWE_POBOCZE 270
#define MAX_ZMNIEJSZENIE_DROGI 80
#define MAX_PRZESUNIECIE_DROGI 100
#define MIN_PRZESUNIECIE_DROGI -100
#define Y_POJAZDU_GRACZA WYSOKOSC_EKRANU / 1.35
#define DUZA_PREDKOSC 6
#define MALA_PREDKOSC 4
#define HAMOWANIE 3
#define PREDKOSC_SKRECANIA 5
#define ZWOLNIJ 0
#define PRZYSPIESZ 1
#define MAX_RUCH_W_BOK_SAMOCHODU 130
#define SRODEK_DROGI 0
#define POJAZD_GRACZA_Y 450
#define ZDERZAK 370
#define BAGAZNIK 520
#define SZEROKOSC_AUTA 35
#define DLUGOSC_AUTA 80
#define DZIESIATA_SEKUNDY 0
#define SEKUNDA 1
#define DWIE_SEKUNDY 2
#define TRZY_SEKUNDY 3
#define PIEC_SEKUND 4
#define MASKA_SAMOCHODU_GRACZA 380
#define POCZATKOWE_X_RADIOWOZU 100
#define POCZATKOWE_Y_RADIOWOZU -150
#define PRZED_GRACZEM -50
#define POZA_MAPA 1
#define ZESTRZELONY 2
#define PIERWSZA 1
#define DRUGA 2
#define CYWILNEGO 1
#define RADIOWOZU 2
#define KOLCZASTEGO 3
#define PIERWSZY 0
#define DRUGI 1
#define TRZECI 2
#define SILA_KOLIZJI 500
#define PREDKOSC_JAZDY_GRACZA 100
#define PREDKOSC_SKRECANIA_GRACZA 100
#define PREDKOSC_NAJEZDZANIA 120
#define PREDKOSC_SKRECANIA_RADIOWOZU 200
#define PR_HAMOWANIA_RADIOWOZU 250
#define PR_PRZYSPIESZANIA_RADIOWOZU 375
#define PREDKOSC_PRZESUWANIA_DROGI 20
#define PREDKOSC_ZMNIEJSZANIA_DROGI 15
#define GRA_NIEZATRZYMANA 0
#define GRA_ZATRZYMANA 1
#define ZA_GRACZEM 700
#define PRZED_GRACZEM -50
#define ZYCIE_CYWILNY 5
#define ZYCIE_KOLCZASTY 7
#define ZYCIE_RADIOWOZ 10

 struct infoGra {
	char wyjdz = NIE;
	char blokadaPunktow = NIE;
	char koniecGry = NIE;
	char zatrzymanieGry = NIE;
	char jazdaPoboczem = NIE;
	char strzelanie = NIE;
	char wyswietlRadiowoz = NIE;
	char wyswietlCywilny = NIE;
	char wyswietlKolczasty = NIE;
	int kierunekPrzesuwaniaMapy = SRODEK;
	int zwezajDroge = NIE;
	char teksturaRadiowozu = PIERWSZA;
	char teksturaStrzelania = PIERWSZA;
	float predkosc = MALA_PREDKOSC;
	float predkosc_x = 0;
	int wynik = 0;
	float przesuniecie = 0;
	float zmniejszenieDrogi = BRAK;
	float ruchSamochodu = 0;
	float pojazd_x = 0;
	char losowoscPrzesuwanieDrogi = 0;
	char losowoscZwezanieDrogi = 0;
	int szerokoscDrogi = 0;
	double czasGry = 0;
	//elementy tablicy czasomierz[] sa zwiekszane w trakcie dzialania programu i gdy osiagna swoja wartosc,
	//sa resetowane z powrotem do 0 dlatego ich wartosc nie moze byc rowna 0 - bo wtedy niektore funkcje,
	//ktore korzystaja z czasomierzy wykonalyby sie odrazu wraz ze starem programu
	double czasomierz[5] = { 0.01 };
	double fps = 0;
	int t1, t2;
	double delta;
	int klatki = 0;
	double licznikFPS = 0;
	SDL_Surface* screen;
	SDL_Surface* charset, * samochodGracza, *pasZieleniLewo, * pasZieleniPrawo,
	*ekranPauzy, *radiowoz1, *radiowoz2, *ogien, *ekranKoniecGry, *strzal1, *strzal2,
	*cywilny1, *cywilny2, *cywilny3, *kolczasty;
};

 struct samochody {
	float radiowoz_x = POCZATKOWE_X_RADIOWOZU;
	float radiowoz_y = POCZATKOWE_Y_RADIOWOZU;
	int radiowoz_zycie = ZYCIE_RADIOWOZ;
	char radiowoz_ruch = NA_SRODEK;
	char radiowoz_zniszczony = NIE;
	//char radiowoz_na_mapie = TAK;
	char radiowoz_poza_mapa = NIE;
	char radiowoz_losowosc_ruchu;
	char radiowoz_losowosc_predkosci;
	float cywilny_x = -100 + (rand() % 200); //losowa liczba od -100 do 100
	float cywilny_y = PRZED_GRACZEM;
	int cywilny_zycie = ZYCIE_CYWILNY;
	char cywilny_zniszczony = NIE;
	char cywilny_rodzaj = PIERWSZY;
	float kolczasty_x = -100 + (rand() % 200);
	float kolczasty_y = PRZED_GRACZEM;
	int kolczasty_zycie = ZYCIE_KOLCZASTY;
	char kolczasty_zniszczony = NIE;
};

 struct kolory {
	int czarny;
	int ciemnoszary;
	int zielony;
	int czerwony;
	int niebieski;
};

 struct strukturaSDL {
	SDL_Event event;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;
	int rc;
};

// narysowanie napisu txt na powierzchni screen, zaczynaj¹c od punktu (x, y)
// charset to bitmapa 128x128 zawieraj¹ca znaki
void narysujNapis(SDL_Surface *screen, int x, int y, const char *text, SDL_Surface *charset) {

	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while(*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
		};
	};

// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt œrodka obrazka sprite na ekranie
void narysujTeksture(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {

	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
	};

//wczytanie obrazka w formacie .bmp
void wczytajObrazek(char nazwa_obrazka[], SDL_Surface** obrazek) {

	*obrazek = SDL_LoadBMP(nazwa_obrazka);
	if (&obrazek == NULL) {
		printf("Error, nie mozna wczytac obrazka z folderu tekstury: %s\n", SDL_GetError());
		SDL_Quit();
	};
}

//dodawanie punktow w zaleznosci od predkosci samochodu
void dodawajPunkty(infoGra* Gra) {

	if (Gra->blokadaPunktow == NIE) {
		if (Gra->jazdaPoboczem == NIE) {
			if (Gra->predkosc > MALA_PREDKOSC) {
				if (Gra->czasomierz[SEKUNDA] == MINELO) {
					Gra->wynik = Gra->wynik + 50;
				}
			}
			else {
				if (Gra->czasomierz[DWIE_SEKUNDY] == MINELO) {
					Gra->wynik = Gra->wynik + 50;
				}
			}
		}
	}

	if (Gra->blokadaPunktow == TAK) {
		if (Gra->czasomierz[PIEC_SEKUND] == MINELO) {
			Gra->blokadaPunktow = NIE;
		}
	}
}

//legenda dotyczaca sterowania w grze
void narysujLegende(infoGra Gra) {

	char text[50];

	sprintf(text, "FPS: %.0lf klatek / s", Gra.fps);
	narysujNapis(Gra.screen, 5, 5, text, Gra.charset);

	sprintf(text, "\030 \032 \031 \033 - sterowanie");
	narysujNapis(Gra.screen, SZEROKOSC_EKRANU - strlen(text) * 8 - 20, WYSOKOSC_EKRANU - 20, text, Gra.charset);

	sprintf(text, "SPACJA - strzelanie");
	narysujNapis(Gra.screen, SZEROKOSC_EKRANU - strlen(text) * 8 - 20, WYSOKOSC_EKRANU - 100, text, Gra.charset);

	sprintf(text, "N - nowa gra");
	narysujNapis(Gra.screen, SZEROKOSC_EKRANU - strlen(text) * 8 - 20, WYSOKOSC_EKRANU - 80, text, Gra.charset);

	sprintf(text, "F - zakoncz gre");
	narysujNapis(Gra.screen, SZEROKOSC_EKRANU - strlen(text) * 8 - 20, WYSOKOSC_EKRANU - 60, text, Gra.charset);

	sprintf(text, "ESC - zamknij program");
	narysujNapis(Gra.screen, SZEROKOSC_EKRANU - strlen(text) * 8 - 20, WYSOKOSC_EKRANU - 40, text, Gra.charset);

	sprintf(text, "Wynik = %d ", Gra.wynik);
	narysujNapis(Gra.screen, Gra.screen->w / 2 - strlen(text) * 8 / 2, 10, text, Gra.charset);

	sprintf(text, "Czas gry: %.1lf ", Gra.czasGry);
	narysujNapis(Gra.screen, Gra.screen->w / 2 - strlen(text) * 8 / 2, 25, text, Gra.charset);

}

//czasomierz sa uzywane do wykonywania innych funkcji w okreslonym odstepie czasu
void resetujczasomierz(infoGra* Gra) {

	if (Gra->czasomierz[DZIESIATA_SEKUNDY] >= 0.1) { Gra->czasomierz[DZIESIATA_SEKUNDY] = 0; }
	if (Gra->czasomierz[SEKUNDA] >= 1) {Gra->czasomierz[SEKUNDA] = 0;}
	if (Gra->czasomierz[DWIE_SEKUNDY] >= 2) {Gra->czasomierz[DWIE_SEKUNDY] = 0;}
	if (Gra->czasomierz[TRZY_SEKUNDY] >= 3) {Gra->czasomierz[TRZY_SEKUNDY] = 0;}
	if (Gra->czasomierz[PIEC_SEKUND] >= 5) {Gra->czasomierz[PIEC_SEKUND] = 0;}
}

//rysowanie podstawowych tekstur na ekranie (pobocze drogi, droga, samochod gracza)
void narysujTekstury(infoGra* Gra, kolory kolor) {

	//wypelniamy mape kolorem drogi (asfaltu)
	SDL_FillRect(Gra->screen, NULL, kolor.ciemnoszary);

	narysujTeksture(Gra->screen, Gra->pasZieleniLewo, 0 + Gra->przesuniecie +Gra->zmniejszenieDrogi, Gra->ruchSamochodu);
	narysujTeksture(Gra->screen, Gra->pasZieleniPrawo, SZEROKOSC_EKRANU + Gra->przesuniecie - Gra->zmniejszenieDrogi, Gra->ruchSamochodu);

	//jesli auto gracza pokona odleglosc wysokosci ekranu to polozenie tekstur bocznych
	//jest wyswietlane od nowa (przesuwaja sie one do gory aby byl efekt poruszania sie samochodu)
	if (Gra->ruchSamochodu > WYSOKOSC_EKRANU) {	Gra->ruchSamochodu = 0;	}

	narysujTeksture(Gra->screen, Gra->samochodGracza, SZEROKOSC_EKRANU / 2 + Gra->pojazd_x, Y_POJAZDU_GRACZA);
}

//przesuwanie drogi na boki
void przesunDroge(char kierunek, infoGra* Gra) {

	//kierunek jest wybierany losowo co 3 sekundy w funkcji losowoPrzesuwajDroge

	if (Gra->koniecGry == NIE) {
		if (kierunek == NA_SRODEK) {
			if (Gra->przesuniecie > BRAK) {
				Gra->przesuniecie = Gra->przesuniecie - PREDKOSC_PRZESUWANIA_DROGI * Gra->predkosc * Gra->delta;
			}
			if (Gra->przesuniecie < BRAK) {
				Gra->przesuniecie = Gra->przesuniecie + PREDKOSC_PRZESUWANIA_DROGI * Gra->predkosc * Gra->delta;
			}
			if (Gra->przesuniecie == BRAK) {
				Gra->kierunekPrzesuwaniaMapy = NIE_PRZESUWAJ;
			}
		}
		if (kierunek == NA_PRAWO) {
			if (Gra->przesuniecie < MAX_PRZESUNIECIE_DROGI) {
				Gra->przesuniecie = Gra->przesuniecie + PREDKOSC_PRZESUWANIA_DROGI * Gra->predkosc * Gra->delta;
			}
			else {
				Gra->kierunekPrzesuwaniaMapy = NIE_PRZESUWAJ;
			}
		}
		if (kierunek == NA_LEWO) {
			if (Gra->przesuniecie > MIN_PRZESUNIECIE_DROGI) {
				Gra->przesuniecie = Gra->przesuniecie - PREDKOSC_PRZESUWANIA_DROGI * Gra->predkosc * Gra->delta;
			}
			else {
				Gra->kierunekPrzesuwaniaMapy = NIE_PRZESUWAJ;
			}
		}
	}
}

//w tej funkcji co trzy sekundy losowany jest kierunek w ktorym przesunie sie droga
void losowoPrzesuwajDroge(infoGra* Gra) {

	if (Gra->czasomierz[TRZY_SEKUNDY] == MINELO) {
		char losowoscPrzesuwanieDrogi = rand() % 3; //losowa liczba od 0 do 2
		if (losowoscPrzesuwanieDrogi == 0) { Gra->kierunekPrzesuwaniaMapy = LEWO; }
		if (losowoscPrzesuwanieDrogi == 1) { Gra->kierunekPrzesuwaniaMapy = PRAWO; }
		if (losowoscPrzesuwanieDrogi == 2) { Gra->kierunekPrzesuwaniaMapy = SRODEK; }
	}
	if (Gra->kierunekPrzesuwaniaMapy == LEWO) { przesunDroge(NA_LEWO, Gra); }
	if (Gra->kierunekPrzesuwaniaMapy == PRAWO) { przesunDroge(NA_PRAWO, Gra); }
	if (Gra->kierunekPrzesuwaniaMapy == SRODEK) { przesunDroge(NA_SRODEK, Gra); }
}

void zmienSzerokoscDrogi(infoGra* Gra) {

	//zmienna zwezajDroge jest ustalana co 3 sekundy w funkcji losowoZwezajDroge

	if (Gra->koniecGry == NIE) {
		if (Gra->zwezajDroge == TAK && Gra->zmniejszenieDrogi < MAX_ZMNIEJSZENIE_DROGI) {

			Gra->zmniejszenieDrogi = Gra->zmniejszenieDrogi + PREDKOSC_ZMNIEJSZANIA_DROGI * Gra->predkosc * Gra->delta;
		}
		else if ((Gra->zwezajDroge == NIE && Gra->zmniejszenieDrogi > BRAK)) {

			Gra->zmniejszenieDrogi = Gra->zmniejszenieDrogi - PREDKOSC_ZMNIEJSZANIA_DROGI * Gra->predkosc * Gra->delta;
		}
	}
}

//w tej funkcji co trzy sekundy losowane jest czy droga ma sie zmniejszyc
void losowoZwezajDroge(infoGra* Gra) {

	//zwezanie drogi odbywa sie w funkcji zmienSzerokoscDrogi

	if (Gra->czasomierz[TRZY_SEKUNDY] == MINELO) {
		char losowoscZwezanieDrogi = rand()%2; //losowa liczba 0 albo 1
		if (losowoscZwezanieDrogi == 0) { Gra->zwezajDroge = TAK; }
		if (losowoscZwezanieDrogi == 1) { Gra->zwezajDroge = NIE; }
	}
	zmienSzerokoscDrogi(Gra);
}

//ustalenie wszystkich kolorow ze struktury Kolory
void ustawKolory(infoGra Gra, kolory* Kolory) {

	Kolory->ciemnoszary = SDL_MapRGB(Gra.screen->format, 0x40, 0x40, 0x40);
	Kolory->czarny = SDL_MapRGB(Gra.screen->format, 0x00, 0x00, 0x00);
	Kolory->zielony = SDL_MapRGB(Gra.screen->format, 0x00, 0xFF, 0x00);
	Kolory->czerwony = SDL_MapRGB(Gra.screen->format, 0xFF, 0x00, 0x00);
	Kolory->niebieski = SDL_MapRGB(Gra.screen->format, 0x11, 0x11, 0xCC);
}

void wczytajWszystkieObrazki(infoGra* Gra) {

	wczytajObrazek("./tekstury/cs8x8.bmp", &Gra->charset);
	SDL_SetColorKey(Gra->charset, true, 0x000000);
	wczytajObrazek("./tekstury/samochod_gracza.bmp", &Gra->samochodGracza);
	wczytajObrazek("./tekstury/koniec_gry.bmp", &Gra->ekranKoniecGry);
	wczytajObrazek("./tekstury/ogien.bmp", &Gra->ogien);
	wczytajObrazek("./tekstury/radiowoz1.bmp", &Gra->radiowoz1);
	wczytajObrazek("./tekstury/radiowoz2.bmp", &Gra->radiowoz2);
	wczytajObrazek("./tekstury/cywilny1.bmp", &Gra->cywilny1);
	wczytajObrazek("./tekstury/cywilny2.bmp", &Gra->cywilny2);
	wczytajObrazek("./tekstury/cywilny3.bmp", &Gra->cywilny3);
	wczytajObrazek("./tekstury/kolczasty.bmp", &Gra->kolczasty);
	wczytajObrazek("./tekstury/pas_zieleni_lewo.bmp", &Gra->pasZieleniLewo);
	wczytajObrazek("./tekstury/pas_zieleni_prawo.bmp", &Gra->pasZieleniPrawo);
	wczytajObrazek("./tekstury/ekran_pauzy.bmp", &Gra->ekranPauzy);
	wczytajObrazek("./tekstury/strzal1.bmp", &Gra->strzal1);
	wczytajObrazek("./tekstury/strzal2.bmp", &Gra->strzal2);
}

void liczFpsy(infoGra* Gra) {

	// delta to czas w sekundach, jaki
	// uplynal od ostatniego narysowania ekranu
	// fpsy wyswietlane sa na ekran w funkcji narysujLegende

	Gra->licznikFPS += Gra->delta;
	if (Gra->licznikFPS > 0.5) {
		Gra->fps = Gra->klatki*2;
		Gra->klatki = 0;
		Gra->licznikFPS -= 0.5;
	};
}

//tryb pelnoekranowy lub okienkowy
void ustawTrybOkienka(char tryb, strukturaSDL* SDL) {

	if (tryb == PELNOEKRANOWY) {
		SDL->rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &SDL->window, &SDL->renderer);
	}
	else if (tryb == OKIENKOWY) {
		SDL->rc = SDL_CreateWindowAndRenderer(SZEROKOSC_EKRANU, WYSOKOSC_EKRANU, 0, &SDL->window, &SDL->renderer);
	}

	if (SDL->rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
	};
}

void zainicjalizujSDL() {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
	}
}

void skonfigurujSDL(strukturaSDL* SDL, infoGra* Gra) {

	//ponizej ustawiamy SDL na odpowiednia rozdzielczosc oraz dodajemy napis u gory okienka

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "closest");
	SDL_RenderSetLogicalSize(SDL->renderer, SZEROKOSC_EKRANU, WYSOKOSC_EKRANU);
	SDL_SetRenderDrawColor(SDL->renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(SDL->window, "Pawel Wawrzynski 193270");
	Gra->screen = SDL_CreateRGBSurface(0, SZEROKOSC_EKRANU, WYSOKOSC_EKRANU, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	SDL->scrtex = SDL_CreateTexture(SDL->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SZEROKOSC_EKRANU, WYSOKOSC_EKRANU);

	// wy³¹czenie widocznoœci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);
}

//ta funkcja wykonywana jest przy wyjsciu z programu
void zwolnijPowierzchnieSDL(strukturaSDL* SDL, infoGra* Gra) {

	SDL_FreeSurface(Gra->samochodGracza);
	SDL_FreeSurface(Gra->pasZieleniLewo);
	SDL_FreeSurface(Gra->pasZieleniPrawo);
	SDL_FreeSurface(Gra->ekranPauzy);
	SDL_FreeSurface(Gra->radiowoz1);
	SDL_FreeSurface(Gra->radiowoz2);
	SDL_FreeSurface(Gra->ogien);
	SDL_FreeSurface(Gra->ekranKoniecGry);
	SDL_FreeSurface(Gra->strzal1);
	SDL_FreeSurface(Gra->strzal2);
	SDL_FreeSurface(Gra->cywilny1);
	SDL_FreeSurface(Gra->cywilny2);
	SDL_FreeSurface(Gra->cywilny3);
	SDL_FreeSurface(Gra->kolczasty);
	SDL_FreeSurface(Gra->charset);
	SDL_FreeSurface(Gra->screen);
	SDL_DestroyTexture(SDL->scrtex);
	SDL_DestroyRenderer(SDL->renderer);
	SDL_DestroyWindow(SDL->window);
	SDL_Quit();
}

//funkcja uzywa SDL
void wyswietlObraz(strukturaSDL* SDL, infoGra* Gra) {

	SDL_UpdateTexture(SDL->scrtex, NULL, (Gra->screen)->pixels, (Gra->screen)->pitch);
	SDL_RenderCopy(SDL->renderer, SDL->scrtex, NULL, NULL);
	SDL_RenderPresent(SDL->renderer);
}

//liczenie czasomierzy oraz calkowitego czasu gry
void liczCzasGry(infoGra* Gra) {

	// czas t2-t1 to czas w milisekundach,
	// jaki uplyna³ od ostatniego narysowania ekranu
	// delta to ten sam czas w sekundach

	Gra->t2 = SDL_GetTicks();
	Gra->delta = (Gra->t2 - Gra->t1) * 0.001;
	Gra->t1 = Gra->t2;

	if (Gra->koniecGry == NIE) {

		if (Gra->zatrzymanieGry == NIE) {

			Gra->czasGry += Gra->delta;

			//ponizej dodajemy czas do czasomierzy
			//sa one uzywane do wykonywania innych funkcji w okreslonym odstepie czasu
			Gra->czasomierz[DZIESIATA_SEKUNDY] += Gra->delta;
			Gra->czasomierz[SEKUNDA] += Gra->delta;
			Gra->czasomierz[DWIE_SEKUNDY] += Gra->delta;
			Gra->czasomierz[TRZY_SEKUNDY] += Gra->delta;
			Gra->czasomierz[PIEC_SEKUND] += Gra->delta;

			//ponizsza funkcja resetuje czasomierz gdy minie okreslony czas (np. sekunda dla czasomierz[SEKUNDA])
			resetujczasomierz(Gra);
		}
	}
}

void sprawdzajCzyGraczJestNaDrodze(infoGra* Gra) {

	//sprawdzamy czy gracz nie zjechal na pobocze (wtedy nie naliczamy punktow)
	if (Gra->pojazd_x < LEWA_GRANICA_DROGI + Gra->przesuniecie + Gra->zmniejszenieDrogi || Gra->pojazd_x > PRAWA_GRANICA_DROGI + Gra->przesuniecie - Gra->zmniejszenieDrogi) {
		Gra->jazdaPoboczem = TAK;
	}
	else {
		Gra->jazdaPoboczem = NIE;
	}

	//sprawdzamy czy gracz nie zjechal za daleko z drogi
	if (Gra->pojazd_x < LEWE_POBOCZE + Gra->przesuniecie + Gra->zmniejszenieDrogi || Gra->pojazd_x > PRAWE_POBOCZE + Gra->przesuniecie - Gra->zmniejszenieDrogi) {
		Gra->koniecGry = TAK;
	}
}

void nowaGra(infoGra* Gra, samochody* Samochody) {

	//ta funkcja resetuje te zmienne do stanu poczatkowego, ktore powinny
	//zostac zresetowane przy rozpoczeciu nowej gry

	Gra->predkosc = MALA_PREDKOSC;
	Gra->wynik = 0;
	Gra->zmniejszenieDrogi = BRAK;
	Gra->przesuniecie = BRAK;
	Gra->szerokoscDrogi = 0;
	Gra->zwezajDroge = NIE;
	Gra->kierunekPrzesuwaniaMapy = SRODEK;
	Gra->czasGry = 0;
	Gra->ruchSamochodu = 0;
	Gra->pojazd_x = 0;
	Gra->koniecGry = NIE;
	Samochody->radiowoz_x = -100 + (rand() % 200); //losowa liczba od -100 do 100
	Samochody->radiowoz_y = POCZATKOWE_Y_RADIOWOZU;
	Samochody->radiowoz_ruch = NA_SRODEK;
	Samochody->radiowoz_zniszczony = NIE;
	Samochody->radiowoz_zycie = ZYCIE_RADIOWOZ;
	Samochody->cywilny_x = -100 + (rand() % 200);
	Samochody->cywilny_y = PRZED_GRACZEM;
	Samochody->cywilny_zycie = ZYCIE_CYWILNY;
	Samochody->cywilny_zniszczony = NIE;
	Samochody->kolczasty_x = -100 + (rand() % 200);
	Samochody->kolczasty_y = PRZED_GRACZEM;
	Samochody->kolczasty_zycie = ZYCIE_KOLCZASTY;
	Samochody->kolczasty_zniszczony = NIE;
}

void zatrzymanieGry(infoGra* Gra, strukturaSDL* SDL) {

	if (Gra->zatrzymanieGry==TAK) {
		while (Gra->zatrzymanieGry == TAK) {

			liczCzasGry(Gra);

			//rysujemy ekran pauzy na srodku ekranu
			narysujTeksture(Gra->screen, Gra->ekranPauzy, SZEROKOSC_EKRANU/2 , WYSOKOSC_EKRANU/2);
			wyswietlObraz(SDL, Gra);

			//sprawdzamy czy gracz wznowil gre wciaskajac p albo czy
			//wylaczyl gre wciskajac ESC
			while (SDL_PollEvent(&SDL->event)) {
				switch (SDL->event.type) {
				case SDL_KEYUP:
					if (SDL->event.key.keysym.sym == SDLK_p) {
						Gra->zatrzymanieGry = NIE;
						Gra->delta = 0;
						break;
					}
					//ponizej zwolnijPowierzchnieSDL - czyli zamknij program
					if (SDL->event.key.keysym.sym == SDLK_ESCAPE) { zwolnijPowierzchnieSDL(SDL, Gra); }
					break;
				case SDL_QUIT:
					zwolnijPowierzchnieSDL(SDL, Gra);
					break;
				}
			}
		}
	}
}

void resetujRadiowoz(char dlaczego, infoGra* Gra, samochody* Samochody) {

	if (Samochody->radiowoz_zniszczony == TAK) {

		narysujTeksture(Gra->screen, Gra->ogien, SZEROKOSC_EKRANU / 2 + Samochody->radiowoz_x + Gra->przesuniecie, Samochody->radiowoz_y);
		if (Gra->czasomierz[DWIE_SEKUNDY]==MINELO) {

			if (dlaczego == ZESTRZELONY) { Gra->wynik = Gra->wynik + 500; }

			Gra->wyswietlRadiowoz = NIE;
			Samochody->radiowoz_x = POCZATKOWE_X_RADIOWOZU;
			Samochody->radiowoz_y = POCZATKOWE_Y_RADIOWOZU;
			Samochody->radiowoz_losowosc_predkosci = PRZYSPIESZ;
			Samochody->radiowoz_zycie = ZYCIE_RADIOWOZ;
			Samochody->radiowoz_zniszczony = NIE;
		}
	}

}

void resetujAuto(float* pojazd_x, float* pojazd_y, int* pojazdZycie, char* pojazdWyswietl, char* pojazdZniszczony, infoGra* Gra) {

	if (*pojazdZniszczony == TAK) {

		narysujTeksture(Gra->screen, Gra->ogien, SZEROKOSC_EKRANU / 2 + *pojazd_x + Gra->przesuniecie, *pojazd_y);
		if (Gra->czasomierz[DWIE_SEKUNDY] == MINELO) {

			*pojazdWyswietl = NIE;
			*pojazd_x = -100+(rand()%200); //losowa liczba od -100 do 100
			*pojazdZycie = ZYCIE_CYWILNY;
			*pojazdZniszczony = NIE;
			if (Gra->predkosc != DUZA_PREDKOSC) {
				*pojazd_y = ZA_GRACZEM;
			}
			else {
				*pojazd_y = PRZED_GRACZEM;
			}
			
		}
	}
}

//obsluga kolizji pomiedzy 2 pojazdami (botami)
void kolizjaPojazdow(float* pojazd1_x, float pojazd1_y, float* pojazd2_x, float pojazd2_y,  infoGra* Gra) {

	//ta funkcja sprawdza czy 2 pojazdy (boty) zderzyly sie ze soba

	if (abs(*pojazd1_x - *pojazd2_x) < SZEROKOSC_AUTA && abs(pojazd1_y - pojazd2_y) < DLUGOSC_AUTA) {
		//uderzamy w samochod z prawej strony
		if (*pojazd1_x >= *pojazd2_x) {
				(*pojazd2_x) = (*pojazd2_x) - SILA_KOLIZJI * (Gra->delta);
				(*pojazd1_x) = (*pojazd1_x) + SILA_KOLIZJI * (Gra->delta);
		}
		//uderzamy w samochod z lewej strony
		if (*pojazd1_x < *pojazd2_x) {
				(*pojazd2_x) = (*pojazd2_x) + SILA_KOLIZJI * (Gra->delta);
				(*pojazd1_x) = (*pojazd1_x) - SILA_KOLIZJI * (Gra->delta);
		}

	}
}

void Strzelanie(int* zycie, float samochod_x, float samochod_y,  infoGra* Gra, samochody* Samochody) {

	//ponizszy kod sprawdza czy przy strzelaniu samochod znajduje sie przed
	//pojazdem gracza, jesli tak - odejmujemy zycie auta

	if (Gra->strzelanie == TAK) {
		if (abs(Gra->pojazd_x - samochod_x - Gra->przesuniecie) < SZEROKOSC_AUTA && (samochod_y < Y_POJAZDU_GRACZA && samochod_y > 0)) {
			if (Gra->czasomierz[DZIESIATA_SEKUNDY] == MINELO) {
				(*zycie)--;
			}
		}
	}

}

//obsluga kolizji pomiedzy pojazdem, a graczem
void kolizjaZGraczem(char kolczasty, float* samochod_x, float samochod_y, infoGra* Gra) {

	//ta funkcja sprawdza czy pojazd na mapie nie zderzyl sie z pojazdem gracza - jesli tak
	//to koordynaty sa odpowiednio zmieniane tak aby uzyskac efekt "przepychania sie"
	//pojazdow na drodze

	if (abs(Gra->pojazd_x - *samochod_x - Gra->przesuniecie) < SZEROKOSC_AUTA && (samochod_y > ZDERZAK && samochod_y < BAGAZNIK)) {
		//uderzamy w samochod z prawej strony
		if (Gra->pojazd_x >= *samochod_x + Gra->przesuniecie) {

			(*samochod_x) = (*samochod_x) - SILA_KOLIZJI * (Gra->delta);
			Gra->pojazd_x = Gra->pojazd_x + SILA_KOLIZJI * 1.2 * (Gra->delta);
		}
		//uderzamy w samochod z lewej strony
		if (Gra->pojazd_x < *samochod_x + Gra->przesuniecie) {

			(*samochod_x) = (*samochod_x) + SILA_KOLIZJI * (Gra->delta);
			Gra->pojazd_x = Gra->pojazd_x - SILA_KOLIZJI * 1.2 * (Gra->delta);
		}
		//uderzenie z tylu (przy duzej predkosci samochod gracza zostaje zniszczony)
		if (samochod_y < MASKA_SAMOCHODU_GRACZA) {
			if (Gra->predkosc == DUZA_PREDKOSC) {
				Gra->koniecGry = TAK;
			}
		}
		//jesli uderzymy w kolczasty pojazd z dowolnej strony - gra sie konczy
		if (kolczasty == TAK) {
			Gra->koniecGry = TAK;
		}
	}
}

void logikaSamochodu(char jakiego, float* x, float* y, int* zycie, char* zniszczony, char* wyswietl,  infoGra* Gra,  samochody* Samochody) {

	//sprawdzamy czy samochod nie zostal zestrzelony
	if (*zycie < 0) {
		*zniszczony = TAK;
		if (jakiego == CYWILNEGO) {
			resetujAuto(x, y, zycie, wyswietl, zniszczony, Gra);
			Gra->blokadaPunktow = TAK;
		}
		else if (jakiego == RADIOWOZU) {
			//Samochody->radiowoz_zniszczony = TAK;
			resetujRadiowoz(ZESTRZELONY, Gra, Samochody);
		}
		else if (jakiego == KOLCZASTEGO) {
			resetujAuto(x, y, zycie, wyswietl, zniszczony, Gra);
		}
	}

	//sprawdzamy czy nie wypadl z drogi
	if (*x < LEWA_GRANICA_DROGI + Gra->zmniejszenieDrogi || *x > PRAWA_GRANICA_DROGI - Gra->zmniejszenieDrogi) {
		*zniszczony = TAK;
		if (jakiego == CYWILNEGO || jakiego == KOLCZASTEGO) {
			resetujAuto(x, y, zycie, wyswietl, zniszczony, Gra);
		}
		else if (jakiego == RADIOWOZU) {
			resetujRadiowoz(POZA_MAPA, Gra, Samochody);
		}
	}

	//sprawdzamy czy pojazd jest za daleko od gracza - wtedy trzeba go zespawnowac od nowa
	if (*y < -100 || *y > 800) {
		if (jakiego == CYWILNEGO || jakiego == KOLCZASTEGO) {
			*zniszczony = TAK;
			resetujAuto(x, y, zycie, wyswietl, zniszczony, Gra);
		}
	}
	if (jakiego == RADIOWOZU && *y > 1200) {
		Samochody->radiowoz_zniszczony = TAK;
		resetujRadiowoz(POZA_MAPA, Gra, Samochody);
	}

	//chcemy aby pojazd kolczasty najezdzal gracza gdy jest blisko naszego samochodu (dlatego jest +/-100)
	if (jakiego == KOLCZASTEGO && *y > ZDERZAK - 100 && *y < BAGAZNIK + 100) {
		if (*x < Gra->pojazd_x - Gra->przesuniecie) {
			*x = *x + PREDKOSC_NAJEZDZANIA * Gra->delta;
		}
		else{
			*x = *x - PREDKOSC_NAJEZDZANIA * Gra->delta;
		}
	}
}

//skrecanie, przyspieszanie, hamowanie radiowozu
void logikaRuchuRadiowozu(infoGra* Gra, samochody* Samochody) {

	if (Gra->koniecGry == NIE) {

		Strzelanie(&Samochody->radiowoz_zycie, Samochody->radiowoz_x, Samochody->radiowoz_y, Gra, Samochody);
		logikaSamochodu(RADIOWOZU, &Samochody->radiowoz_x, &Samochody->radiowoz_y, &Samochody->radiowoz_zycie, &Samochody->radiowoz_zniszczony, &Gra->wyswietlRadiowoz, Gra,Samochody);
		kolizjaZGraczem(NIE, &Samochody->radiowoz_x, Samochody->radiowoz_y, Gra);

		if (Gra->czasomierz[TRZY_SEKUNDY] == MINELO) {
			//wartosc ponizszych losowych zmiennych ma wplyw na to w jaki
			//sposob bedzie zachowywal sie samochod na drodze
			Samochody->radiowoz_losowosc_ruchu = rand() % 3; //liczba od 0 do 2
			Samochody->radiowoz_losowosc_predkosci = rand() % 2; //liczba 0 albo 1
		}

		if (Samochody->radiowoz_zniszczony == NIE) {
			if (Samochody->radiowoz_losowosc_ruchu == NA_LEWO) {
				if (Samochody->radiowoz_x < MAX_RUCH_W_BOK_SAMOCHODU) {
					Samochody->radiowoz_x = Samochody->radiowoz_x + PREDKOSC_SKRECANIA_RADIOWOZU * Gra->delta;
				}
			}
			if (Samochody->radiowoz_losowosc_ruchu == NA_PRAWO) {
				if (Samochody->radiowoz_x > -MAX_RUCH_W_BOK_SAMOCHODU) {
					Samochody->radiowoz_x = Samochody->radiowoz_x - PREDKOSC_SKRECANIA_RADIOWOZU * Gra->delta;
				}
			}
			if (Samochody->radiowoz_losowosc_ruchu == NA_SRODEK) {
				if (Samochody->radiowoz_x > SRODEK_DROGI) {
					Samochody->radiowoz_x = Samochody->radiowoz_x - PREDKOSC_SKRECANIA_RADIOWOZU/2 * Gra->delta;
				}
				if (Samochody->radiowoz_x < SRODEK_DROGI) {
					Samochody->radiowoz_x = Samochody->radiowoz_x + PREDKOSC_SKRECANIA_RADIOWOZU/2 * Gra->delta;
				}
			}

			if (Samochody->radiowoz_losowosc_predkosci == ZWOLNIJ) {
				if (Samochody->radiowoz_y < POJAZD_GRACZA_Y) {
					//hamowanie radiowozu
					Samochody->radiowoz_y = Samochody->radiowoz_y + PR_HAMOWANIA_RADIOWOZU * Gra->delta;
					if (Gra->predkosc == HAMOWANIE) {
						//dodatkowe hamowanie radiowozu - bo my tez hamujemy
						Samochody->radiowoz_y = Samochody->radiowoz_y + PR_HAMOWANIA_RADIOWOZU/2 * Gra->delta;
					}
				}
			}
			if (Samochody->radiowoz_losowosc_predkosci == PRZYSPIESZ) {
				// ponizej jest POJAZD_GRACZA_Y -50 poniewaz chcemy aby samochod zajezdzal droge graczowi 
				// (czyli musi byc lekko przed naszym pojazdem)
				if (Samochody->radiowoz_y > POJAZD_GRACZA_Y - 50) {
					//przyspieszanie radiowozu
					Samochody->radiowoz_y = Samochody->radiowoz_y - PR_PRZYSPIESZANIA_RADIOWOZU * Gra->delta;
				}
			}
		}
	}
}

//co sekunde zmienia teksture radiowozu (swiatlo niebieskie/czerwone)
void animacjaSyrenyPolicyjnej(infoGra* Gra, samochody* Samochody) {


	if (Gra->czasomierz[SEKUNDA] == MINELO) {
		if (Gra->teksturaRadiowozu == PIERWSZA) { Gra->teksturaRadiowozu = DRUGA; }
		else { Gra->teksturaRadiowozu = PIERWSZA; }
	}

	if (Gra->teksturaRadiowozu == PIERWSZA) {
		narysujTeksture(Gra->screen, Gra->radiowoz1, SZEROKOSC_EKRANU / 2 + Samochody->radiowoz_x + Gra->przesuniecie, Samochody->radiowoz_y);
	}
	if (Gra->teksturaRadiowozu == DRUGA) {
		narysujTeksture(Gra->screen, Gra->radiowoz2, SZEROKOSC_EKRANU / 2 + Samochody->radiowoz_x + Gra->przesuniecie, Samochody->radiowoz_y);
	}
}

void losowoSpawnujRadiowoz(infoGra* Gra, samochody* Samochody) {

	if (Gra->wyswietlRadiowoz == NIE) {

		//jesli pojazd nie jest wyswietlony to co okreslony czas
		//pojazd moze sie pojawic na mapie (ale nie musi, jest to losowe)

		if (Gra->czasomierz[DWIE_SEKUNDY] == MINELO) {
			Gra->wyswietlRadiowoz = rand() % 2; // 0 (NIE) albo 1 (TAK)
		}
	}

	if (Gra->wyswietlRadiowoz == TAK) {

		animacjaSyrenyPolicyjnej(Gra, Samochody);

		//jesli pojazd jest wyswietlony to musimy obslugiwac jego logike w ponizszej funkcji
		logikaRuchuRadiowozu(Gra, Samochody);
	}

}

void losowoSpawnujSamochody(infoGra* Gra, samochody* Samochody) {

		if (Gra->wyswietlCywilny == NIE) {

			if (Gra->czasomierz[SEKUNDA] == MINELO) {
				Gra->wyswietlCywilny = rand() % 2; // losowa liczba 0 (NIE) albo 1 (TAK)
				Samochody->cywilny_rodzaj = rand() % 3; // losowa liczba 0,1 albo 2
			}
		}
		if (Gra->wyswietlCywilny == TAK) {
			if (Samochody->cywilny_rodzaj == PIERWSZY) {
				narysujTeksture(Gra->screen, Gra->cywilny1, SZEROKOSC_EKRANU / 2 + Samochody->cywilny_x + Gra->przesuniecie, Samochody->cywilny_y);
			}
			if (Samochody->cywilny_rodzaj == DRUGI) {
				narysujTeksture(Gra->screen, Gra->cywilny2, SZEROKOSC_EKRANU / 2 + Samochody->cywilny_x + Gra->przesuniecie, Samochody->cywilny_y);
			}
			if (Samochody->cywilny_rodzaj == TRZECI) {
				narysujTeksture(Gra->screen, Gra->cywilny3, SZEROKOSC_EKRANU / 2 + Samochody->cywilny_x + Gra->przesuniecie, Samochody->cywilny_y);
			}
			if (Gra->koniecGry == NIE) {
				Strzelanie(&Samochody->cywilny_zycie, Samochody->cywilny_x, Samochody->cywilny_y, Gra, Samochody);
				kolizjaZGraczem(NIE, &Samochody->cywilny_x, Samochody->cywilny_y, Gra);
				kolizjaPojazdow(&Samochody->radiowoz_x, Samochody->radiowoz_y, &Samochody->cywilny_x, Samochody->cywilny_y, Gra);
				kolizjaPojazdow(&Samochody->kolczasty_x, Samochody->kolczasty_y, &Samochody->cywilny_x, Samochody->cywilny_y, Gra);
				logikaSamochodu(CYWILNEGO, &Samochody->cywilny_x, &Samochody->cywilny_y, &Samochody->cywilny_zycie, &Samochody->cywilny_zniszczony, &Gra->wyswietlCywilny, Gra, Samochody);
			}
		}

		if (Gra->wyswietlKolczasty == NIE) {
			if (Gra->czasomierz[DWIE_SEKUNDY] == MINELO) {
				Gra->wyswietlKolczasty = rand() % 2; // 0 (NIE) albo 1 (TAK)
			}
		}
		if (Gra->wyswietlKolczasty == TAK) {
			narysujTeksture(Gra->screen, Gra->kolczasty, SZEROKOSC_EKRANU / 2 + Samochody->kolczasty_x + Gra->przesuniecie, Samochody->kolczasty_y);
			if (Gra->koniecGry == NIE) {
				Strzelanie(&Samochody->kolczasty_zycie, Samochody->kolczasty_x, Samochody->kolczasty_y, Gra, Samochody);
				kolizjaZGraczem(TAK, &Samochody->kolczasty_x, Samochody->kolczasty_y, Gra);
				kolizjaPojazdow(&Samochody->radiowoz_x, Samochody->radiowoz_y, &Samochody->kolczasty_x, Samochody->kolczasty_y, Gra);
				kolizjaPojazdow(&Samochody->cywilny_x, Samochody->cywilny_y, &Samochody->kolczasty_x, Samochody->kolczasty_y, Gra);
				logikaSamochodu(KOLCZASTEGO, &Samochody->kolczasty_x, &Samochody->kolczasty_y, &Samochody->kolczasty_zycie, &Samochody->kolczasty_zniszczony, &Gra->wyswietlKolczasty, Gra, Samochody);
			}
		}

}

void sprawdzajCzyKoniecGry(infoGra* Gra) {

	if (Gra->koniecGry == TAK) {

		//na nasz pojazd nakladana jest tekstura ognia
		narysujTeksture(Gra->screen, Gra->ogien, SZEROKOSC_EKRANU / 2 + Gra->pojazd_x, WYSOKOSC_EKRANU / 1.35);
		//samochod gracza zatrzymuje sie
		Gra->predkosc = 0;
		//blokujemy mozliwosc strzelania
		Gra->strzelanie = NIE;

		//na srodku ekranu pojawia sie napis informujacy o koncu gry
		narysujTeksture(Gra->screen, Gra->ekranKoniecGry, SZEROKOSC_EKRANU / 2, WYSOKOSC_EKRANU / 2);
	}
}

void animacjaStrzelania(infoGra* Gra) {

	//ponizszy kod wykonywany jest gdy gracz trzyma spacjê (przycisk strzelania)

	if (Gra->strzelanie == TAK) {

		// co 1/10 sekundy zmieniamy teksture ognia z karabinu tak aby stworzyc
		// animacje ognia wylotowego

		if (Gra->czasomierz[DZIESIATA_SEKUNDY] == MINELO) {
			if (Gra->teksturaStrzelania == PIERWSZA) { Gra->teksturaStrzelania = DRUGA; }
			else { Gra->teksturaStrzelania = PIERWSZA; }
		}

		if (Gra->teksturaStrzelania == PIERWSZA) {
			// ponizsza tekstura wyswietlana musi byc w odpowiednim miejscu, dlatego do polozenia pojazdu
			// dodane sa odpowiednio: 12,35 oraz 12,40 - te wartosci mozna zmienic, zmieni sie wtedy
			// polozenie tekstury ognia wylotowego
			narysujTeksture(Gra->screen, Gra->strzal2, SZEROKOSC_EKRANU / 2 + Gra->pojazd_x + 12, WYSOKOSC_EKRANU / 1.35 - 35);
			narysujTeksture(Gra->screen, Gra->strzal2, SZEROKOSC_EKRANU / 2 + Gra->pojazd_x - 12, WYSOKOSC_EKRANU / 1.35 - 35);
		}
		if (Gra->teksturaStrzelania == DRUGA) {
			narysujTeksture(Gra->screen, Gra->strzal1, SZEROKOSC_EKRANU / 2 + Gra->pojazd_x + 12, WYSOKOSC_EKRANU / 1.35 - 40);
			narysujTeksture(Gra->screen, Gra->strzal1, SZEROKOSC_EKRANU / 2 + Gra->pojazd_x - 12, WYSOKOSC_EKRANU / 1.35 - 40);
		}
	}
}

//wspolczynniki (d,m,h) maja wplyw na to jak szybko auta beda sie od nas oddalaly gdy gracz jedzie mala predkoscia albo
//duza predkoscia albo hamuje - jesli zwiekszymy wspolczynnik to ruch pionowy innych aut wzgledem nas bedzie szybszy
void kontrolujPredkosc(int d, int m, int h, float* pojazd_y, infoGra* Gra, samochody* Samochody) {

	if (Gra->predkosc == DUZA_PREDKOSC) { *pojazd_y += Gra->predkosc * d * Gra->delta; }
	if (Gra->predkosc == MALA_PREDKOSC) { *pojazd_y -= Gra->predkosc * m * Gra->delta; }
	if (Gra->predkosc == HAMOWANIE) { *pojazd_y -= Gra->predkosc * h * Gra->delta; }
}

void obslugaKlawiszy(infoGra* Gra, strukturaSDL* SDL, samochody* Samochody) {

	// obs³uga zdarzeñ (o ile jakieœ zasz³y)

	while (SDL_PollEvent(&SDL->event)) {
		switch (SDL->event.type) {
		case SDL_KEYDOWN:
			if (SDL->event.key.keysym.sym == SDLK_DOWN) { Gra->predkosc = HAMOWANIE; }
			if (SDL->event.key.keysym.sym == SDLK_UP) { Gra->predkosc = DUZA_PREDKOSC; }
			if (SDL->event.key.keysym.sym == SDLK_RIGHT && Gra->predkosc != 0) { Gra->predkosc_x = PREDKOSC_SKRECANIA; }
			if (SDL->event.key.keysym.sym == SDLK_LEFT && Gra->predkosc != 0) { Gra->predkosc_x = -PREDKOSC_SKRECANIA; }
			if (SDL->event.key.keysym.sym == SDLK_SPACE) { Gra->strzelanie = TAK; }
			break;
		case SDL_KEYUP:
			if (SDL->event.key.keysym.sym == SDLK_DOWN) { Gra->predkosc = MALA_PREDKOSC; }
			if (SDL->event.key.keysym.sym == SDLK_UP) { Gra->predkosc = MALA_PREDKOSC; }
			if (SDL->event.key.keysym.sym == SDLK_RIGHT) { Gra->predkosc_x = 0; }
			if (SDL->event.key.keysym.sym == SDLK_LEFT) { Gra->predkosc_x = 0; }
			if (SDL->event.key.keysym.sym == SDLK_n) { nowaGra(Gra, Samochody); }
			if (SDL->event.key.keysym.sym == SDLK_f) { nowaGra(Gra, Samochody); Gra->zatrzymanieGry = TAK; }
			if (SDL->event.key.keysym.sym == SDLK_p) { Gra->zatrzymanieGry = TAK; }
			if (SDL->event.key.keysym.sym == SDLK_SPACE) { Gra->strzelanie = NIE; }
			if (SDL->event.key.keysym.sym == SDLK_ESCAPE) { Gra->wyjdz = TAK; }
			break;
		case SDL_QUIT:
			Gra->wyjdz = TAK;
			break;
		};
	};
}

//predkosc samochodu gracza i innych pojazdow na mapie
void kontrolujRuchPredkosc(infoGra* Gra, samochody* Samochody) {

	Gra->pojazd_x += Gra->predkosc_x * PREDKOSC_SKRECANIA_GRACZA * Gra->delta;

	//zmienna ruchSamochodu jest uzywana do wyswietlania tekstur trawy po obu stronach drogi, do
	//wspolrzednej y tekstury dodawana jest ta wartosc, dzieki czemu otoczenie przesuwa sie do dolu
	Gra->ruchSamochodu += Gra->predkosc * PREDKOSC_JAZDY_GRACZA * Gra->delta;

	if (Gra->wyswietlRadiowoz == TAK) { kontrolujPredkosc(30,20,80,&(Samochody->radiowoz_y), Gra, Samochody); }
	if (Gra->wyswietlCywilny == TAK) { kontrolujPredkosc(40,10,90,&(Samochody->cywilny_y), Gra, Samochody); }
	if (Gra->wyswietlKolczasty == TAK) { kontrolujPredkosc(60, 5, 90, &(Samochody->kolczasty_y), Gra, Samochody); }
}

void ograniczFPS(char ograniczenie, int ileFps, infoGra* Gra) {

	if (ograniczenie == TAK) {

		if(Gra->delta < 1000/ileFps)
		{
			SDL_Delay(1000/ileFps - Gra->delta);
		}
	}
}


#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv) {
	srand(time(NULL));
	infoGra Gra;
	kolory Kolory;
	strukturaSDL SDL;
	samochody Samochody;

	zainicjalizujSDL();
	//ustawTrybOkienka(OKIENKOWY, &SDL);
	ustawTrybOkienka(PELNOEKRANOWY, &SDL);
	skonfigurujSDL(&SDL, &Gra);
	wczytajWszystkieObrazki(&Gra);
	ustawKolory(Gra, &Kolory);

	//czas t1 jest uzywany w funkcji liczCzasGry
	Gra.t1 = SDL_GetTicks();

	while (Gra.wyjdz==NIE) {

			ograniczFPS(TAK, 160, &Gra);
			zatrzymanieGry(&Gra, &SDL);
			liczCzasGry(&Gra);
			liczFpsy(&Gra);
			losowoPrzesuwajDroge(&Gra);
			losowoZwezajDroge(&Gra);
			narysujTekstury(&Gra, Kolory);
			losowoSpawnujRadiowoz(&Gra, &Samochody);
			losowoSpawnujSamochody(&Gra, &Samochody);
			animacjaStrzelania(&Gra);
			sprawdzajCzyKoniecGry(&Gra);
			narysujLegende(Gra);
			wyswietlObraz(&SDL, &Gra);
			dodawajPunkty(&Gra);
			sprawdzajCzyGraczJestNaDrodze(&Gra);
			kontrolujRuchPredkosc(&Gra, &Samochody);

			obslugaKlawiszy(&Gra, &SDL, &Samochody);

			Gra.klatki++;
	};

	zwolnijPowierzchnieSDL(&SDL, &Gra);

	return 0;
}