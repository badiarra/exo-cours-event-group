#include <Arduino.h>
#include <cstring>


#include "led/Led.h"
#include "pot/Pot.h"
#include "bouton/Button.h"

// Broches
#define PIN_POT       A0
#define PIN_BOUTON    2
#define PIN_ROUGE     8
#define PIN_VERTE     9

// Bits du groupe d'evenements
#define POT_BIT       (1U << 0)  // bit 0  
#define BOUTON_BIT    (1U << 1)  // bit 1
#define TOUS_BITS     (POT_BIT | BOUTON_BIT)

const char* DEL_ROUGE = "ROUGE";
const char* DEL_VERTE = "VERTE";

const int SEUIL = 2000;


// Groupe d'evenements partage
EventGroupHandle_t groupe;

// Objets
Led rouge(PIN_ROUGE, DEL_ROUGE);
Led verte(PIN_VERTE, DEL_VERTE);

Pot potentiometre(PIN_POT, SEUIL);
Button bouton(PIN_BOUTON);


// TACHE 1 : lecture du potentiometre

void vTaskPot(void *parametre) {
    for (;;) {
        if (potentiometre.isActive()) {
            xEventGroupSetBits(groupe, POT_BIT);
        } else {
            xEventGroupClearBits(groupe, POT_BIT);
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}


// TACHE 2 : lecture du bouton

void vTaskButton(void *parametre) {
    for (;;) {
        if (bouton.isPressed()) {
            xEventGroupSetBits(groupe, BOUTON_BIT);
        } else {
            xEventGroupClearBits(groupe, BOUTON_BIT);
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}


// TACHE 3 : commande des DEL

void vTaskLed(void *parametre) {

   EventBits_t bits;
    for (;;) {
       bits = xEventGroupWaitBits(
            groupe,
            TOUS_BITS,
            pdFALSE,  // Ne pas effacer les bits
            pdFALSE,  // Attendre au moins un bit
            0         // Ne pas bloquer
        );

        // On pourrait utiliser xEventGroupGetBits() qui est plus direct et simple ici

        bool potActif = (bits & POT_BIT) != 0;

        bool boutonActif = (bits & BOUTON_BIT) != 0;

        if (potActif && boutonActif) {
            // Les deux entrees actives
            verte.on();
            rouge.off();
        }
        else if (potActif != boutonActif) {  // else if (potActif || boutonActif) {
            // Une seule entree active
            verte.off();
            rouge.on();
        }
        else {
            // Aucune entree active
            verte.off();
            rouge.off();
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

/* void vTaskLed(void *param) {

    Del *led = (Del *)param;

    for (;;) {

        EventBits_t bits = xEventGroupWaitBits(
            groupe,
            POT_BIT | BOUTON_BIT,  // Bits attendus
            pdFALSE,               // Ne pas effacer les bits
            pdFALSE,                // Ne pas attendre tous les deux bits
            0                       // Ne pas attendre / bloquer
        );

        bool potActif = (bits & POT_BIT) != 0;
        bool boutonActif = (bits & BOUTON_BIT) != 0;

        bool etat;

        if (strcmp(led->getName(), VERTE) == 0) {
            etat = potActif && boutonActif;
        } else {
            etat = potActif != boutonActif;
        }

        if (etat) {
            led->on();
        } else {
            led->off();
        }

                vTaskDelay(pdMS_TO_TICKS(20));
    }
} */

void setup() {

    // Initialiser les objets
    rouge.init();
    verte.init();
    potentiometre.init();
    bouton.init();

    // Creer le groupe d'evenements
    groupe = xEventGroupCreate();

    if (groupe == NULL) {
        while (true) {
            // Erreur de creation
        }
    }

    // Creer les trois taches
    xTaskCreate(
        vTaskPot,
        "Pot",
        2048,
        NULL,
        1,
        NULL
    );

    xTaskCreate(
        vTaskButton,
        "Bouton",
        2048,
        NULL,
        1,
        NULL
    );

    xTaskCreate(
        vTaskLed,
        "DEL",
        2048,
        NULL,
        1,
        NULL
    );
}

void loop() {
    vTaskDelay(portMAX_DELAY);
}