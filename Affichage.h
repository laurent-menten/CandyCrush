#pragma once

#include "Matrice.h"

void EffaceEcran();

void AffichePlateau( Plateau* plateau, bool avecInfos = true );

void AfficheMessage(const char* error, ...);
void AfficheAvertissement(const char* error, ...);
void AfficheErreur( const char* error, ... );

void DebugAfficeCase(Plateau* plateau, int c, int l);
