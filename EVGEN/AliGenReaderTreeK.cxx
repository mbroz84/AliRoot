/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/*
$Log$
*/
#include <TFile.h>
#include <TTree.h>
#include <TParticle.h>

#include "AliGenReaderTreeK.h"
#include "AliStack.h"
#include "AliHeader.h"
#include "AliRun.h"

ClassImp(AliGenReaderTreeK);


AliGenReaderTreeK::AliGenReaderTreeK():AliGenReader() 
{
//  Default constructor
    fFileName       = NULL;
    fStack          = 0;
    fHeader         = 0;
    fNcurrent       = 0;
    fNparticle      = 0;
    fFile           = 0;
    fBaseFile       = 0;
}

void AliGenReaderTreeK::Init() 
{
// Initialization
// Connect base file and file to read from

    TTree *ali = gAlice->TreeE();
    if (ali) {
	fBaseFile = ali->GetCurrentFile();
    } else {
	printf("\n Warning: Basefile cannot be found !\n");
    }
    fFile = new TFile(fFileName);
}

Int_t AliGenReaderTreeK::NextEvent() 
{
// Read the next event  
//  cd to file with old kine tree    
    if (!fBaseFile) Init();
    if (fStack) delete fStack;
    fStack = new AliStack(1000);
    fFile->cd();
//  Connect treeE
    TTree* treeE = (TTree*)gDirectory->Get("TE");
    treeE->ls();
    if (fHeader) delete fHeader;
    fHeader = 0;
    treeE->SetBranchAddress("Header", &fHeader);
//  Get next event
    treeE->GetEntry(fNcurrent);
    fStack = fHeader->Stack();
    fStack->GetEvent(fNcurrent);
    
//  cd back to base file
    fBaseFile->cd();
//
    fNcurrent++;
    fNparticle = 0;
    Int_t ntrack =  fStack->GetNtrack();
    printf("\n Next event contains %d particles", ntrack);
    
    return  ntrack;
}

TParticle* AliGenReaderTreeK::NextParticle() 
{
//  Return next particle
    TParticle* part = fStack->Particle(fNparticle);
    fNparticle++;
    return part;
}






