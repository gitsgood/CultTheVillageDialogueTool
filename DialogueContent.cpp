#include "DialogueContent.h"
#include "JsonFileManagerStatics.h"

std::string DialogueNode::StringifyForJson() const
{
	std::ostringstream JsonNodeObject;
	JsonNodeObject
		<< "\t\t{\n"
		<< "\t\t\t\"NodeId\": \"" << JsonFileManagerStatics::ParserForJson(NodeId) << "\",\n"
		<< "\t\t\t\"NPCDialogue\": \"" << JsonFileManagerStatics::ParserForJson(NPCDialogue) << "\",\n"
		<< "\t\t\t\"OutgoingEdgeIds\": [";
	{
		size_t SizeOfEdgeVector{ OutgoingEdgeIds.size() };
		for (size_t i = 0; i < SizeOfEdgeVector; i++)
		{
			JsonNodeObject
				<< "\"" << JsonFileManagerStatics::ParserForJson(OutgoingEdgeIds[i]) << "\"";
			if (i != SizeOfEdgeVector - size_t(1))
			{
				JsonNodeObject << ", ";
			}
		}
	}
	JsonNodeObject
		<< "],\n"
		<< "\t\t\t\"bIsConvinced\": " << JsonFileManagerStatics::BoolParserForJson(bIsConvinced) << ",\n"
		<< "\t\t\t\"bIsAngered\": " << JsonFileManagerStatics::BoolParserForJson(bIsAngered) << ",\n"
		<< "\t\t\t\"bIsAntagonistic\": " << JsonFileManagerStatics::BoolParserForJson(bIsAntagonistic) << ",\n"
		<< "\t\t\t\"bIsVillagerKilled\": " << JsonFileManagerStatics::BoolParserForJson(bIsVillagerKilled) << "\n"
		<< "\t\t}";

	return JsonNodeObject.str();
}

std::string DialogueEdge::StringifyForJson() const
{
	std::ostringstream JsonEdgeObject;
	JsonEdgeObject
		<< "\t\t{\n"
		<< "\t\t\t\"EdgeId\": \"" << JsonFileManagerStatics::ParserForJson(EdgeId) << "\",\n"
		<< "\t\t\t\"PlayerDialogue\": \"" << JsonFileManagerStatics::ParserForJson(PlayerDialogue) << "\",\n"
		<< "\t\t\t\"NextNodeId\": \"" << JsonFileManagerStatics::ParserForJson(NextNodeId) << "\",\n"

		<< "\t\t\t\"bIsConvinced\": " << JsonFileManagerStatics::BoolParserForJson(bIsConvinced) << ",\n"
		<< "\t\t\t\"bIsAngered\": " << JsonFileManagerStatics::BoolParserForJson(bIsAngered) << ",\n"
		<< "\t\t\t\"bIsAntagonistic\": " << JsonFileManagerStatics::BoolParserForJson(bIsAntagonistic) << ",\n"
		<< "\t\t\t\"bIsVillagerKilled\": " << JsonFileManagerStatics::BoolParserForJson(bIsVillagerKilled) << "\n"
		<< "\t\t}";

	return JsonEdgeObject.str();
}

void DialogueTree::PopulateMaps()
{
	auto PopulateLamda = []<typename T>(const std::vector<T>&V, std::unordered_map<std::string, T>&M)
	{
		M.clear();
		for (const T& Member : V)
		{
			M.try_emplace(Member.GetId(), Member);
		}
	};

	PopulateLamda(DialogueEdgeContainer, EdgeMap);

	PopulateLamda(DialogueNodeContainer, NodeMap);
}

void DialogueTree::EdgeInput(const std::string& inEdgeId, const std::string& inPreviousReply, const int inLocalIndex, const int inLocalTotal)
{
	CoreHelpers::ClearScreen();
	std::cout
		<< "Player reply " << inLocalIndex << " out of " << inLocalTotal << " in response to:\n"
		<< "\t\"" << inPreviousReply << "\"\n"
		<< "\nWhat is the player's say?\n";
	std::string DialogueContent{ "" };
	std::getline(std::cin, DialogueContent);

	std::cout
		<< "\nDoes the dialogue end here?\n"
		<< "\t\t[Y]: Yes\t[N]: No\n";
	std::string NextNodeId{ "" };
	NextNodeId = CoreHelpers::YesOrNo() ? "" : "Node" + std::to_string(NodeCount);

	DialogueEdge ThisEdge(inEdgeId, DialogueContent, NextNodeId);
	FlagSetter(ThisEdge);

	DialogueEdgeContainer.push_back(ThisEdge);

	if (NextNodeId != "")
	{
		NodeInput(NextNodeId, DialogueContent);
	}
}

void DialogueTree::NodeInput(const std::string& inNodeId, const std::string& inPreviousReply)
{
	CoreHelpers::ClearScreen();
	NodeCount++;

	if (inNodeId == "START")
	{
		std::cout << "What does " << NPCId << " say?\n\n";
	}
	else
	{
		std::cout
			<< "What does " << NPCId << " say in reply to:\n"
			<< "\t\"" << inPreviousReply << "\"\n";
	}
	std::string DialogueContent{ "" };
	std::getline(std::cin, DialogueContent);

	std::cout << "\nHow many replies would the player have to the above?\n";
	int ReplyCount{ 0 };
	CoreHelpers::SafeIntegerInput(ReplyCount);
	std::vector<std::string> OutgoingEdgeIds;
	OutgoingEdgeIds.reserve(ReplyCount);
	for (int i = 0; i < ReplyCount; i++)
	{
		OutgoingEdgeIds.emplace_back("Edge" + std::to_string(EdgeCount));
		EdgeCount++;
	}

	DialogueNode ThisNode(DialogueContent, OutgoingEdgeIds, inNodeId);
	FlagSetter(ThisNode);

	DialogueNodeContainer.push_back(ThisNode);

	int BirthedEdgeCount{ 0 };
	for (const std::string& Id : OutgoingEdgeIds)
	{
		BirthedEdgeCount++;
		EdgeInput(Id, DialogueContent, BirthedEdgeCount, ReplyCount);
	}
}

template<typename T>
void DialogueTree::FlagSetter(T& DialogueObject)
{
	std::cout
		<< "\nDo you wish to implement dialogue flags here?\n"
		<< "(If you say no, all flags are set to false)\n"
		<< "\t\t[Y]: Yes\t[N]: No\n";
	if (CoreHelpers::YesOrNo())
	{
		std::cout
			<< "\nIs the villager convinced to support you?\n"
			<< "\t\t[Y]: Yes\t[N]: No\n";
		DialogueObject.bIsConvinced = CoreHelpers::YesOrNo();

		std::cout
			<< "\nIs the villager angered by you?\n"
			<< "\t\t[Y]: Yes\t[N]: No\n";
		DialogueObject.bIsAngered = CoreHelpers::YesOrNo();

		std::cout
			<< "\nIs the villager antagonised by you?\n"
			<< "\t\t[Y]: Yes\t[N]: No\n";
		DialogueObject.bIsAntagonistic = CoreHelpers::YesOrNo();

		std::cout
			<< "\nDid you just MURDER this person??\n"
			<< "\t\t[Y]: Yes\t[N]: No\n";
		DialogueObject.bIsVillagerKilled = CoreHelpers::YesOrNo();
	}
	else
	{
		DialogueObject.bIsConvinced = false;
		DialogueObject.bIsAngered = false;
		DialogueObject.bIsAntagonistic = false;
		DialogueObject.bIsVillagerKilled = false;
	}
}