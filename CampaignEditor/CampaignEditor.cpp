/**
 * @author Julian D'Addario
 */

#include "CampaignEditor.h"
#include "MapEditor.h"
#include <sstream>
#include "../Character/Character.h"

CampaignEditor::CampaignEditor() = default;

int CampaignEditor::getUserInput() {
    std::cout << "Campaign Editing Commands:" << '\n';
    std::cout << " 1 - Add Map" << '\n';
    std::cout << " 2 - Remove Map" << '\n';
    std::cout << " 3 - Edit Map" << '\n';
    std::cout << " 4 - Save and Exit" << '\n';
    std::cout << " 5 - Load Campaign" << '\n';
    std::cout << "-1 - Exit Without Saving\n";
    std::cout << "Enter command number: ";
    int op {};
    std::cin >> op;
    return op;
}

void CampaignEditor::addMap() {
    MapEditor mapEdit = MapEditor();
    mapEdit.run();
    campaign.addMap(mapEdit.getMap());
    mapFiles.push_back(mapEdit.getFileName());
}

void CampaignEditor::removeMap() {
    std::cout << "Enter number of map to remove: ";
    int mapNum {};
    std::cin >> mapNum;

    campaign.removeMap(mapNum);
    mapFiles.erase(mapFiles.begin()+mapNum);
}

void CampaignEditor::editMap() {
    std::cout << "Enter number of map to edit: ";
    int mapNum {};
    std::cin >> mapNum;

    MapEditor mapEdit = MapEditor();

    mapEdit.setMap(campaign.getMap(mapNum));
    campaign.getMap(mapNum)->notify();
    mapEdit.edit();
}

void CampaignEditor::saveCampaign() {
    std::cout << "Enter Campaign File Name (ex: grimdark): ";
    std::string file {};
    std::cin >> file;
    file += ".txt";
    save(file);
    std::cout << "Campaign saved as " << file << '\n';
}

void CampaignEditor::save(const std::string& filename) {
    std::ofstream output{"../CampaignSaves/"+filename};
    if(!output) {
        std::cerr << "Can not save " << filename << ".\n";
        std::exit(1);
    }

    for(std::string f : mapFiles)
        output << f << '\n';
}

Campaign CampaignEditor::loadCampaignFromFile(std::string file) {
    std::ifstream input("../CampaignSaves/"+file);
    std::string line, data;

    std::vector<std::string> loadedMapFiles;
    Campaign loadedCampaign;
    while(std::getline(input, line)) {
        loadedMapFiles.push_back(line);
    }

    MapDirector director {};
    for (std::string f : loadedMapFiles) {
        MapBuilder* mapBuilder = new DefaultMapBuilder(f);

        director.setMapBuilder(mapBuilder);
        director.constructMap();

        MapObserver* observer { new MapObserver(director.getMap()) };

        loadedCampaign.addMap(director.getMap());
    }

    return loadedCampaign;
}

void CampaignEditor::loadCampaign(/*Character *player*/) {
    std::cout << "Enter Campaign filename.txt to load: ";
    std::string file {};
    std::cin >> file;

    std::ifstream input("../CampaignSaves/"+file);
    std::string line, data;

    mapFiles.clear();
    campaign.clear();
    while(std::getline(input, line)) {
        mapFiles.push_back(line);
    }

    MapDirector director {};
    for (std::string f : mapFiles) {
        MapBuilder* mapBuilder = new DefaultMapBuilder(f);

        director.setMapBuilder(mapBuilder);
        director.constructMap();

        MapObserver* observer { new MapObserver(director.getMap()) };

        campaign.addMap(director.getMap());
    }
    //campaign.setPlayer(player);
    //runCampaign();
}

void CampaignEditor::loadCampaign(Character *player) {
    std::cout << "Enter Campaign filename.txt to load: ";
    std::string file {};
    std::cin >> file;

    std::ifstream input("../CampaignSaves/"+file);
    std::string line, data;

    mapFiles.clear();
    campaign.clear();
    while(std::getline(input, line)) {
        mapFiles.push_back(line);
    }

    MapDirector director {};
    for (std::string f : mapFiles) {
        MapBuilder* mapBuilder = new DefaultMapBuilder(f);

        director.setMapBuilder(mapBuilder);
        director.constructMap();
        campaign.addMap(director.getMap());
    }
    campaign.setPlayer(player);
    runCampaign();
}

void CampaignEditor::run(/*Character *player*/) {
    bool editing = true;
    while(editing) {
        clearConsole();
        printCampaignDetails();
        int input {getUserInput()};
        switch(input) {
            case 1:
                addMap();
                break;
            case 2:
                removeMap();
                break;
            case 3:
                editMap();
                break;
            case 4:
                saveCampaign();
                editing = false;
                break;
            case 5:
                loadCampaign(/*player*/);
                break;
            case -1:
                return;
            default:
                std::cout << "Invalid command, try again.\n";
                break;
        }
        if (input == 4)
            break;
    }
}

void CampaignEditor::run(Character *player) {
    bool editing = true;
    while(editing) {
        printCampaignDetails();
        int input {getUserInput()};
        switch(input) {
            case 1:
                addMap();
                break;
            case 2:
                removeMap();
                break;
            case 3:
                editMap();
                break;
            case 4:
                saveCampaign();
                editing = false;
                break;
            case 5:
                loadCampaign(player);
                break;
            case -1:
                return;
            default:
                std::cout << "Invalid command, try again.\n";
                break;
        }
        if (input == 4)
            break;
    }
}

void CampaignEditor::printCampaignDetails() {
    std::cout << "\nCurrent Campaign:\n";
    if(mapFiles.empty())
        std::cout << " no maps, please add a map";
    else {
        for(int i {0}; i < mapFiles.size(); i++) {
            std::cout << " Map " << i << " - " << mapFiles[i] << '\n';
        }
    }
    std::cout << '\n';
}

Campaign CampaignEditor::getCampaign() {
    return campaign;
}

void CampaignEditor::runCampaign() {
    campaign.startCampaign();
    campaign.run();
}
