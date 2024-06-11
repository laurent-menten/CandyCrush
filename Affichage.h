#pragma once

#include "Matrice.h"

void EffaceEcran();

void AffichePlateau( Plateau* plateau, bool avecInfos = true );

void AfficheMessage(const char* message, ...);
void AfficheAvertissement(const char* messageAvertissement, ...);
void AfficheErreur( const char* messageErreur, ... );
