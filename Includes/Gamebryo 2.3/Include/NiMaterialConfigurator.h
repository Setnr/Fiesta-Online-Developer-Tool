// EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1996-2007 Emergent Game Technologies.
//      All Rights Reserved.
//
// Emergent Game Technologies, Chapel Hill, North Carolina 27517
// http://www.emergent.net

#ifndef NIMATERIALCONFIGURATOR_H
#define NIMATERIALCONFIGURATOR_H

#include "NiGPUProgram.h"
#include "NiString.h"
#include "NiFixedString.h"
#include "NiTSet.h"
#include "NiMaterialResourceProducerNode.h"
#include "NiMaterialResourceConsumerNode.h"
#include "NiMaterialResourceBinding.h"
#include "NiMaterialFragmentNode.h"
#include "NiGPUProgramCache.h"

class NIMAIN_ENTRY NiMaterialConfigurator : public NiRefObject
{
public:
    NiMaterialConfigurator(NiGPUProgramCache* pkCache);
    virtual ~NiMaterialConfigurator(){};

    unsigned int GetNodeCount();
    NiMaterialNode* GetNode(unsigned int uiNode);
    void AddNode(NiMaterialNode* pkNode);

    unsigned int GetBindingCount();
    NiMaterialResourceBinding* GetBinding(unsigned int uiBind);
    NiMaterialNode* GetNodeContainingResource(NiMaterialResource* pkRes);
    NiMaterialNode* GetNodeByName(const NiFixedString& kName, 
        unsigned int uiOccurance = 0);
    bool AddBinding(NiMaterialResourceBinding* pkBinding);
    bool AddBinding(NiMaterialResource* pkOutputRes,
        NiMaterialResource* pkInputRes);
    bool AddBinding(const NiFixedString& kOutputVar, 
        NiMaterialNode* pkOutputNode,
        const NiFixedString& kInputVar,
        NiMaterialNode* pkInputNode);
    bool AddBinding(NiMaterialResource* pkOutRes,
        const NiFixedString& kInputVar,
        NiMaterialNode* pkInputNode);
    bool AddBinding(const NiFixedString& kOutputVar, 
        NiMaterialNode* pkOutputNode,
        NiMaterialResource* pkInRes);

    virtual NiGPUProgram* Evaluate(const char* pcName, 
        NiGPUProgram::ProgramType eType, 
        NiTObjectPtrSet<NiMaterialResourcePtr>& kUniforms);

    const NiGPUProgramCache* GetGPUProgramCache() const;
    const NiFixedString& GetPlatformString() const;

    virtual void Reset();

    virtual bool CanBindTypes(const NiFixedString& kSrcType, 
        const NiFixedString& kDestType);

    void SetDescription(const char* pcDescription);
protected:
    virtual bool CanBind(NiMaterialResourceBinding* pkBinding);

    NiMaterialResourceBinding* GetOutputBindingForInput(
        NiMaterialResource* pkRes, unsigned int uiWhichOne = 0);
    NiMaterialResourceBinding* GetInputBindingForOutput(
        NiMaterialResource* pkRes, unsigned int uiWhichOne = 0);
    void SetVariableNameForAllOutputBindings(NiMaterialResource* pkRes,
        const NiFixedString& kVariable, bool bOverwriteExisting = true);
    void SetVariableNameForAllInputBindings(NiMaterialResource* pkRes,
        const NiFixedString& kVariable, bool bOverwriteExisting = true);

    NiString FormatVariableDeclaration(NiMaterialResource* pkResource,
        bool bAddSemanticAndLabel = true);
    NiString FormatType(const NiFixedString& kStr);
    NiString GenerateFunctionString(NiMaterialFragmentNode* pkNode,
        const NiTObjectPtrSet<NiFixedString>* pkValidTargets);
    NiString GenerateInOutStruct(
        const NiTObjectPtrSet<NiMaterialResourcePtr>& kInputs);
    NiString GenerateFunctionCall(NiMaterialFragmentNode* pkNode,
        unsigned int uiUniqueIdentifier);
    NiString GetVariableArgumentNameForCall(NiMaterialResource* pkRes, 
        NiMaterialNode* pkNode, bool bIsInput, bool bGoToStart);
    NiString ResolvePassthroughVariables(
        NiMaterialResourceConsumerNode* pkNode);

    NiTObjectArray<NiMaterialNodePtr> m_kNodes;
    NiTObjectArray<NiMaterialResourceBindingPtr> m_kBindings;

    NiString m_kDescription;
    NiFixedString m_kPlatform;

    NiGPUProgramCachePtr m_spCache;
};

typedef NiPointer<NiMaterialConfigurator> NiMaterialConfiguratorPtr;

#endif  //#ifndef NIMATERIALCONFIGURATOR_H
