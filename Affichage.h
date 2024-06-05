#pragma once

#include "Matrice.h"

void EffaceEcran();

void AffichePlateau( Plateau* plateau, bool avecInfos = true );
void AfficheAvertissement(const char* error, ...);
void AfficheErreur( const char* error, ... );
