#pragma once

#include "Matrice.h"

void EffaceEcran();

void AffichePlateau( Plateau* plateau );

void AfficheAvertissement(const char* error, ...);
void AfficheErreur( const char* error, ... );
