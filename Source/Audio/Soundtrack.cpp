//
// Soundtrack.cpp
//
// Clark Kromenaker
//
#include "Soundtrack.h"

#include "StringUtil.h"

#include "Audio.h"
#include "IniParser.h"
#include "Services.h"

int WaitNode::Execute()
{
    // Don't execute if we've hit the repeat limit.
    // Otherwise, increment the execution count (execution IS happening!)
    if(repeat > 0 && repeat - executionCount <= 0) { return 0; }
    executionCount++;
    
    // Do random check. If it fails, we don't execute.
    // But note execution count is still incremented!
    int randomCheck = rand() % 100 + 1;
    if(randomCheck > random) { return 0; }
    
    // We will execute this node. Decide wait time based on min/max.
    if(minWaitTimeMs == maxWaitTimeMs) { return minWaitTimeMs; }
    if(maxWaitTimeMs == 0 && minWaitTimeMs > 0) { return minWaitTimeMs; }
    if(maxWaitTimeMs != 0 && minWaitTimeMs > maxWaitTimeMs) { return minWaitTimeMs; }
    
    // Normal case - random between min and max.
    return (rand() % maxWaitTimeMs + minWaitTimeMs);
}

int SoundNode::Execute()
{
    // Don't execute if we've hit the repeat limit.
    // Otherwise, increment the execution count (execution IS happening!)
    if(repeat > 0 && repeat - executionCount <= 0) { return 0; }
    executionCount++;
    
    // Do random check. If it fails, we don't execute.
    // But note execution count is still incremented!
    int randomCheck = rand() % 100 + 1;
    if(randomCheck > random) { return 0; }
    
    // Definitely want to play the sound, if it exists.
    Audio* audio = Services::GetAssets()->LoadAudio(soundName);
    if(audio == nullptr) { return 0; }
    audio->SetIsMusic(true);
    
    // Play the audio correctly.
    if(is3d)
    {
        std::cout << "Play 3D! " << position << std::endl;
        Services::GetAudio()->Play3D(audio, position, minDist, maxDist);
    }
    else
    {
        Services::GetAudio()->Play(audio, fadeInTimeMs);
    }
    
    // Return audio length. Gotta convert seconds to milliseconds.
    return (int)(audio->GetDuration() * 1000.0f);
}

Soundtrack::Soundtrack(std::string name, char* data, int dataLength) : Asset(name)
{
    ParseFromData(data, dataLength);
}

void Soundtrack::ParseFromData(char *data, int dataLength)
{
    IniParser parser(data, dataLength);
    IniSection section;
    std::vector<SoundNode*> prsSoundNodes;
    while(parser.ReadNextSection(section))
    {
        // If this isn't a PRS node, and we have just parsed one or more PRS nodes,
        // we need to generate the PRS node and add it to our list.
        if(prsSoundNodes.size() > 0
           && !StringUtil::EqualsIgnoreCase(section.name, "PRS"))
        {
            PrsNode* prsNode = new PrsNode();
            prsNode->soundNodes = prsSoundNodes;
            mNodes.push_back(prsNode);
            prsSoundNodes.clear();
        }
        
        if(StringUtil::EqualsIgnoreCase(section.name, "WAIT"))
        {
            // Parse wait node keys and add to nodes list.
            WaitNode* node = new WaitNode();
            for(auto& line : section.lines)
            {
				IniKeyValue& entry = line.entries[0];
                if(StringUtil::EqualsIgnoreCase(entry.key, "MinWaitMs"))
                {
                    node->minWaitTimeMs = entry.GetValueAsInt();
                }
                else if(StringUtil::EqualsIgnoreCase(entry.key, "MaxWaitMs"))
                {
                    node->maxWaitTimeMs = entry.GetValueAsInt();
                }
                else if(StringUtil::EqualsIgnoreCase(entry.key, "Repeat"))
                {
                    node->repeat = entry.GetValueAsInt();
                }
                else if(StringUtil::EqualsIgnoreCase(entry.key, "Random"))
                {
                    node->random = entry.GetValueAsInt();
                }
                else
                {
                    std::cout << "Unexpected key: " << entry.key << std::endl;
                }
            }
            mNodes.push_back(node);
        }
        else if(StringUtil::EqualsIgnoreCase(section.name, "SOUND"))
        {
            // Parse sound node and add to node list.
            SoundNode* node = ParseSoundNodeFromSection(section);
            mNodes.push_back(node);
        }
        else if(StringUtil::EqualsIgnoreCase(section.name, "PRS"))
        {
            // Parse sound node.
            SoundNode* soundNode = ParseSoundNodeFromSection(section);
            
            // Repeat and loop are ignored for PRS.
            soundNode->repeat = 0;
            soundNode->loop = false;
            
            // Add it to prs sound nodes.
            prsSoundNodes.push_back(soundNode);
        }
        else if(StringUtil::EqualsIgnoreCase(section.name, "SOUNDTRACK"))
        {
            for(auto& line : section.lines)
            {
				IniKeyValue& entry = line.entries[0];
                if(StringUtil::EqualsIgnoreCase(entry.key, "SoundType"))
                {
                    // Set soundtype.
					//TODO
                }
                else
                {
                    std::cout << "Unexpected key: " << entry.key << std::endl;
                }
            }
        }
        else
        {
            std::cout << "Unexpected section name: " << section.name << std::endl;
        }
    }
}

SoundNode* Soundtrack::ParseSoundNodeFromSection(IniSection& section)
{
    SoundNode* node = new SoundNode();
    for(auto& line : section.lines)
    {
		IniKeyValue& entry = line.entries[0];
        if(StringUtil::EqualsIgnoreCase(entry.key, "Name"))
        {
            node->soundName = entry.value;
        }
        else if(StringUtil::EqualsIgnoreCase(entry.key, "Volume"))
        {
            node->volume = entry.GetValueAsInt();
        }
        else if(StringUtil::EqualsIgnoreCase(entry.key, "Repeat"))
        {
            node->repeat = entry.GetValueAsInt();
        }
        else if(StringUtil::EqualsIgnoreCase(entry.key, "Random"))
        {
            node->random = entry.GetValueAsInt();
        }
        else if(StringUtil::EqualsIgnoreCase(entry.key, "Loop"))
        {
            node->loop = entry.GetValueAsInt();
        }
        else if(StringUtil::EqualsIgnoreCase(entry.key, "FadeInMs"))
        {
            node->fadeInTimeMs = entry.GetValueAsInt();
        }
        else if(StringUtil::EqualsIgnoreCase(entry.key, "StopMethod"))
        {
            node->stopMethod = entry.GetValueAsInt();
        }
        else if(StringUtil::EqualsIgnoreCase(entry.key, "FadeOutMs"))
        {
            node->fadeOutTimeMs = entry.GetValueAsInt();
        }
        else if(StringUtil::EqualsIgnoreCase(entry.key, "3D"))
        {
            node->is3d = entry.GetValueAsInt();
        }
        else if(StringUtil::EqualsIgnoreCase(entry.key, "MinDist"))
        {
            node->minDist = entry.GetValueAsInt();
        }
        else if(StringUtil::EqualsIgnoreCase(entry.key, "MaxDist"))
        {
            node->maxDist = entry.GetValueAsInt();
        }
        else if(StringUtil::EqualsIgnoreCase(entry.key, "X"))
        {
			node->position.x = entry.GetValueAsFloat();
        }
        else if(StringUtil::EqualsIgnoreCase(entry.key, "Y"))
        {
            node->position.y = entry.GetValueAsFloat();
        }
        else if(StringUtil::EqualsIgnoreCase(entry.key, "Z"))
        {
            node->position.z = entry.GetValueAsFloat();
        }
        else if(StringUtil::EqualsIgnoreCase(entry.key, "Follow"))
        {
            node->followModelName = entry.value;
        }
        else
        {
            std::cout << "Unexpected key: " << entry.key << std::endl;
        }
    }
    return node;
}
