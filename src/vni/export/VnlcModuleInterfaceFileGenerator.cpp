#include "VnlcModuleInterfaceFileGenerator.hpp"
#include "../../ast/declaration/VnlcClassDeclarationNode.hpp"
#include "../../ast/declaration/VnlcFunctionDeclarationNode.hpp"
#include "../../ast/declaration/VnlcValueDeclarationNode.hpp"
#include "nlohmann/json_fwd.hpp"
#include <cassert>

VnlcModuleInterfaceFileGenerator::VnlcModuleInterfaceFileGenerator(
    std::vector<const VnlcDeclarationNode*>&& declarationNodes,
    std::vector<std::string>&& importedAliases,
    const VnlcConfig& config,
    const VnlcSemanticAnalysisResult& semantic
) noexcept
    : declarationNodes(std::move(declarationNodes)),
      importedAliases(std::move(importedAliases)),
      config(config),
      semantic(semantic) {}

nlohmann::json VnlcModuleInterfaceFileGenerator::stringifyMetadata(const std::vector<VnlcDeclarationItem::MetadataTerm>& metadataTerms) {
    nlohmann::json metadataObj = nlohmann::json::object();

    for (const auto& metadataTerm : metadataTerms) {
        metadataObj.emplace(metadataTerm.key->getIdentifierString(), metadataTerm.value);
    }

    return metadataObj;
}

nlohmann::json VnlcModuleInterfaceFileGenerator::stringifyGenericParameters(const std::vector<std::unique_ptr<VnlcIdentifierNode>>& genericParameterNames) {
    nlohmann::json genericParametersObj = nlohmann::json::array();

    for (const auto& genericParam : genericParameterNames) {
        genericParametersObj.push_back(genericParam->getIdentifierString());
    }

    return genericParametersObj;
}

nlohmann::json VnlcModuleInterfaceFileGenerator::stringifyVariable(const VnlcValueDeclarationNode* variable) {
    nlohmann::json variableObj = nlohmann::json::object();
    variableObj.emplace("category", "let");

    const auto& metadata = variable->getMetadataTerms();
    if (!metadata.empty()) {
        variableObj.emplace("metadata", stringifyMetadata(metadata));
    }

    const auto& typeNode = variable->getType();
    if (typeNode.has_value()) {
        const auto& semanticType = semantic.getSemanticTypeByTypeNode(typeNode.value().get());
        if (semanticType.has_value()) {
            variableObj.emplace("type", semanticType.value()->getFullTypeName());
        }
    } else {
        const auto& inferredType = semantic.getInferredValueType(variable);
        if (inferredType.has_value()) {
            variableObj.emplace("type", inferredType.value()->getFullTypeName());
        }
    }

    return variableObj;
}

nlohmann::json VnlcModuleInterfaceFileGenerator::stringifyFunction(const VnlcFunctionDeclarationNode* function) {
    nlohmann::json functionObj = nlohmann::json::object();
    functionObj.emplace("category", "func");

    const auto& metadata = function->getMetadataTerms();

    if (!metadata.empty()) {
        functionObj.emplace("metadata", stringifyMetadata(metadata));
    }

    const auto& returnTypeNode = function->getReturnType();
    if (returnTypeNode.has_value()) {
        const auto& semanticType = semantic.getSemanticTypeByTypeNode(returnTypeNode.value().get());
        if (semanticType.has_value()) {
            functionObj.emplace("returnType", semanticType.value()->getFullTypeName());
        }
    } else {
        const auto& inferredReturnType = semantic.getInferredFunctionReturnType(function);
        if (inferredReturnType.has_value()) {
            functionObj.emplace("returnType", inferredReturnType.value()->getFullTypeName());
        }
    }

    nlohmann::json parametersObj = nlohmann::json::object();
    for (const auto& parameter : function->getParameters()) {
        nlohmann::json parameterObj;
        const auto& paramTypeNode = parameter->getType();
        if (paramTypeNode.has_value()) {
            const auto& semanticType = semantic.getSemanticTypeByTypeNode(paramTypeNode.value().get());
            if (semanticType.has_value()) {
                parameterObj.emplace("type", semanticType.value()->getFullTypeName());
            }
        } else {
            const auto& inferredParamType = semantic.getInferredValueType(parameter.get());
            if (inferredParamType.has_value()) {
                parameterObj.emplace("type", inferredParamType.value()->getFullTypeName());
            }
        }

        parametersObj.emplace(parameter->getName().getIdentifierString(), parameterObj);
    }

    functionObj.emplace("parameters", parametersObj);
    functionObj.emplace("native", function->getKind() == VnlcFunctionDeclarationType::Kind::NATIVE);

    return functionObj;
}

nlohmann::json VnlcModuleInterfaceFileGenerator::stringifyClass(const VnlcClassDeclarationNode* classNode) {
    nlohmann::json classObj = nlohmann::json::object();
    classObj.emplace("category", "class");

    const auto& metadata = classNode->getMetadataTerms();
    if (!metadata.empty()) {
        classObj.emplace("metadata", stringifyMetadata(metadata));
    }

    std::optional<std::string> baseClassName;
    if (classNode->getBaseClass().has_value()) {
        const auto& baseClassTypeNode = classNode->getBaseClass().value().get();
        const auto& semanticType = semantic.getSemanticTypeByTypeNode(baseClassTypeNode);
        if (semanticType.has_value()) {
            baseClassName = semanticType.value()->getFullTypeName();
        }
    }
    classObj.emplace("baseClass", baseClassName);

    std::vector<std::string> implementedInterfaceNames;
    for (const auto& interfaceTypeNode : classNode->getImplementedInterfaces()) {
        const auto& semanticType = semantic.getSemanticTypeByTypeNode(interfaceTypeNode.get());
        if (semanticType.has_value()) {
            implementedInterfaceNames.emplace_back(semanticType.value()->getFullTypeName());
        }
    }
    classObj.emplace("implementedInterfaces", implementedInterfaceNames);

    classObj.emplace("final", classNode->isFinal());

    classObj.emplace("genericParameters", stringifyGenericParameters(classNode->getGenericParameterNames()));

    nlohmann::json propertiesObj = nlohmann::json::object();
    nlohmann::json methodsObj = nlohmann::json::object();

    for (const auto& memberDeclaration : classNode->getMemberDeclarations()) {
        if (const auto* property = dynamic_cast<const VnlcValueDeclarationNode*>(memberDeclaration.get())) {
            propertiesObj.emplace(property->getName().getIdentifierString(), stringifyProperty(property));
        } else if (const auto* method = dynamic_cast<const VnlcFunctionDeclarationNode*>(memberDeclaration.get())) {
            methodsObj.emplace(method->getName().getIdentifierString(), stringifyMethod(method));
        }
    }

    classObj.emplace("properties", propertiesObj);
    classObj.emplace("methods", methodsObj);

    return classObj;
}

nlohmann::json VnlcModuleInterfaceFileGenerator::stringifyInterface(const VnlcInterfaceDeclarationNode* interfaceNode) {
    nlohmann::json interfaceObj = nlohmann::json::object();
    interfaceObj.emplace("category", "method");

    const auto& metadata = interfaceNode->getMetadataTerms();
    if (!metadata.empty()) {
        interfaceObj.emplace("metadata", stringifyMetadata(metadata));
    }

    interfaceObj.emplace("genericParameters", stringifyGenericParameters(interfaceNode->getGenericParameterNames()));

    nlohmann::json methodsObj = nlohmann::json::object();
    for (const auto& method : interfaceNode->getMethodDeclarations()) {
        nlohmann::json methodObj = stringifyMethod(method.get());
        methodsObj.emplace(method->getName().getIdentifierString(), methodObj);
    }
    interfaceObj.emplace("methods", methodsObj);

    return interfaceObj;
}

nlohmann::json VnlcModuleInterfaceFileGenerator::stringifyEnum(const VnlcEnumDeclarationNode* enumNode) {
    nlohmann::json enumObj = nlohmann::json::object();
    enumObj.emplace("category", "enum");

    enumObj.emplace("genericParameters", stringifyGenericParameters(enumNode->getGenericParameterNames()));

    nlohmann::json membersObj = nlohmann::json::object();
    for (const auto& member : enumNode->getMemberDeclarations()) {
        nlohmann::json memberObj = stringifyEnumMember(member.get());
        membersObj.emplace(member->getName().getIdentifierString(), memberObj);
    }
    enumObj.emplace("members", membersObj);

    return enumObj;
}