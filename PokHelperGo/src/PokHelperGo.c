/*
 ============================================================================
 Name        : PokHelperGo.c
 Author      : Maarti
 Version     : 1.0
 Copyright   : 2016
 Description : Hello World in GTK+
 ============================================================================
 */

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>	// strcopy, strcat

// Structures :
typedef struct entry_data
{
	GtkWidget *nbRoucoolEntry;
	GtkWidget *nbBonbonEntry;
	GtkWidget *resultLabel;
} entry_data;

// Prototypes :
void calcul_evo(int nbPokemon, int nbBonbon, GtkLabel *resultLabel);
void evoluer(int *stockPokemon,int *stockBonbon, int *nbTransfert, int *nbEvo, int *fini);
static void on_click_calculer (GtkWidget *widget, gpointer data);

// Methods :
int main (int   argc, char *argv[]) {
	GtkBuilder *builder;
	GObject *window;
	GObject *button;
	entry_data* ed = g_malloc(sizeof(*ed));
	GtkAdjustment *adjustment;

	gtk_init (&argc, &argv);

	/* Construct a GtkBuilder instance and load our UI description */
	builder = gtk_builder_new ();
	gtk_builder_add_from_file (builder, "./src/builder.ui", NULL);

	/* Connect signal handlers to the constructed widgets. */
	window = gtk_builder_get_object (builder, "window");
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	// On recupere les Widgets
	ed->nbRoucoolEntry = (GtkWidget*) gtk_builder_get_object (builder, "nbRoucoolEntry");
	ed->nbBonbonEntry = (GtkWidget*) gtk_builder_get_object (builder, "nbBonbonEntry");
	ed->resultLabel =  (GtkWidget*) gtk_builder_get_object (builder, "resultLabel");


	// On associe des bornes aux Spin Buttons
	adjustment = gtk_adjustment_new (0.0, 0.0, 750.0, 1.0, 3.0, 0.0);
	gtk_spin_button_set_adjustment (GTK_SPIN_BUTTON(ed->nbRoucoolEntry), adjustment);
	adjustment = gtk_adjustment_new (0.0, 0.0, 9999.0, 1.0, 10.0, 0.0);
	gtk_spin_button_set_adjustment (GTK_SPIN_BUTTON(ed->nbBonbonEntry), adjustment);

	// On connect les callback
	button = gtk_builder_get_object (builder, "calculer");
	g_signal_connect (button, "clicked", G_CALLBACK (on_click_calculer), (gpointer) ed);
	button = gtk_builder_get_object (builder, "quit");
	g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);

	gtk_main ();

	free(ed);
	return 0;
}

// Click on "calculate"
static void on_click_calculer (GtkWidget *widget, gpointer data) {
	entry_data* ed = (entry_data*)data;
	GtkLabel *resultLabel = GTK_LABEL(ed->resultLabel);

	int nbRoucool = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(ed->nbRoucoolEntry));
	int nbBonbon = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(ed->nbBonbonEntry));
	g_print ("--calcul--\nNb pidgey = %d	-	Nb candies = %d\n",nbRoucool,nbBonbon);

	calcul_evo(nbRoucool,nbBonbon,resultLabel);
}

// Calcule le nombre de transferts nécessaire en fonction des
// données saisies et met à jour le label passé en paramètre
void calcul_evo(int nbPokemon, int nbBonbon, GtkLabel *resultLabel){
	int nbEvo = nbBonbon / 12;
	int resteBonbonApresEvo = nbBonbon % 12;
	int restePokApresEvo = nbPokemon - nbEvo;
	int nbTransfert = 0;
	char resultStr[200], nbEvoStr[20], nbTrsfStr[20];


	if (nbBonbon+nbPokemon>12 && nbPokemon>=1){

		if(nbEvo>=nbPokemon) {									// Bonbons suffisants pour tous les pokemon
			//g_print ("Vous pouvez faire évoluer vos %d Roucool sans en transférer.\n",nbPokemon);
			g_print ("You can evolve your %d Pidgey(s) without any transfer.\n",nbPokemon);
			snprintf(nbEvoStr, 20,"%d",nbPokemon);
			//strcpy(resultStr, "Vous pouvez faire évoluer \nvos ");
			strcpy(resultStr, "You can evolve your ");
			strcat(resultStr, nbEvoStr);
			//strcat(resultStr, " Roucool sans en transférer.");
			strcat(resultStr, " Pidgey(s) \nwithout any transfer.");
			gtk_label_set_text (resultLabel, resultStr);

		}else if((restePokApresEvo+resteBonbonApresEvo)<13){	// Evolutions sans transfert
			g_print ("Vous pouvez faire évoluer %d de vos Roucool, pas de transfert requis.\n",nbEvo);
			snprintf(nbEvoStr, 20,"%d",nbEvo);
			//strcpy(resultStr, "Vous pouvez faire évoluer ");
			strcpy(resultStr, "You can evolve ");
			strcat(resultStr, nbEvoStr);
			//strcat(resultStr, " de \nvos Roucool, pas de transert requis.");
			strcat(resultStr, " of \nyour Pidgey(s), no transfer required.");
			gtk_label_set_text (resultLabel, resultStr);
		}else{													// Transferts avant évolution
			int stockPokemon = nbPokemon;
			int stockBonbon = resteBonbonApresEvo;
			int fini = 0;
			do {
				g_print ("--LOG-- Evo %2d	Trsfrt %2d	Pokmn %2d	Bnbn %2d\n",nbEvo,nbTransfert,stockPokemon,stockBonbon);
				evoluer(&stockPokemon, &stockBonbon,  &nbTransfert, &nbEvo, &fini);
			}
			while(fini==0);
			g_print ("En transférant %d Roucool, vous pourrez en faire évoluer %d\n",nbTransfert,nbEvo);
			snprintf(nbTrsfStr, 20,"%d",nbTransfert);
			snprintf(nbEvoStr, 20,"%d",nbEvo);
			//strcpy(resultStr, "En transférant ");
			strcpy(resultStr, "By transferring ");
			strcat(resultStr, nbTrsfStr);
			//strcat(resultStr, " Roucool, \nvous pourrez en faire évoluer ");
			strcat(resultStr, " Pidgey(s), \nyou could evolve ");
			strcat(resultStr, nbEvoStr);
			strcat(resultStr, ".");
			gtk_label_set_text (resultLabel, resultStr);
		}

	}else{														// Pas d'évolution possible
		//gtk_label_set_text (resultLabel, "Pas assez de Roucool.");
		gtk_label_set_text (resultLabel, "Not enough Pidgeys.");
	}

}

// Tente de transférer des pokemon pour pouvoir evoluer et met à jour les stocks
// Si non => rollback
void evoluer(int *stockPokemon,int *stockBonbon, int *nbTransfert, int *nbEvo, int *fini){
	// on conserve les valeurs pour pouvoir rollback
	int nbTinit = *nbTransfert;
	int nbEinit = *nbEvo;

	// on fait evoluer
	if ((*stockBonbon+*stockPokemon)>=12){
		int nbTransfertForThisEvo = 12-*stockBonbon;
		*nbTransfert = *nbTransfert + nbTransfertForThisEvo;
		*stockPokemon = *stockPokemon - nbTransfertForThisEvo;
		*stockBonbon = 0;
		(*nbEvo)++;
	}else
		*fini = 1;

	// on test si le nb de pokemon n'est plus suffisant, dans ce cas, => rollback
	if (*nbEvo > *stockPokemon){
		*nbEvo = nbEinit;
		*nbTransfert = nbTinit;
		*fini = 1;
	}
}

// Credit Pidgey icon :
// http://kattling.deviantart.com/art/FREE-Bouncy-Pidgey-Icon-333607385

// Credit candy icon :
// https://github.com/OrangeeWeb/Pokemon
// https://www.reddit.com/r/pokemongo/comments/4ulm2k/i_made_a_pokemon_go_candy_image_generator_change/

