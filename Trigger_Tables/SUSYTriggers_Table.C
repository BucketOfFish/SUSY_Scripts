#include "../CommonLibraries.C"
#include <boost/algorithm/string/replace.hpp>

using namespace std;

void SUSYTriggers_Table() {

    vector<string> file_names {"SUSY2_Data/data15-16", "SUSY2_Data/data17", "SUSY2_Data/data18", "SUSY2_Signal_mc16a/C1N2_WZ_600_0_2L2J", "SUSY2_Signal_mc16cd/C1N2_WZ_600_0_2L2J", "SUSY2_Signal_mc16e/C1N2_WZ_600_0_2L2J", "SUSY2_Signal_mc16a/C1N2_WZ_200p0_100p0_2L2J", "SUSY2_Signal_mc16cd/C1N2_WZ_200p0_100p0_2L2J", "SUSY2_Signal_mc16e/C1N2_WZ_200p0_100p0_2L2J"};
    vector<string> file_labels {"data15-16", "data17", "data18", "(600, 0) MC16a", "(600, 0) MC16cd", "(600, 0) MC16e", "(200, 100) MC16a", "(200, 100) MC16cd", "(200, 100) MC16e"};
    vector<string> triggers {"1", "trigMatch_1L2LTrig", "trigMatch_1L2LTrigOR", "trigMatch_2LTrigOR"};
    vector<string> flavors {"lepFlavor[0]==1 && lepFlavor[1]==1", "lepFlavor[0]==2 && lepFlavor[1]==2", "((lepFlavor[0]==2 && lepFlavor[1]==1) || (lepFlavor[0]==1 && lepFlavor[1]==2))"};

    //--- make LaTeX table
    ofstream fout("table.tex", ios::out);
    fout << "\\documentclass{article}" << endl;
    fout << "\\usepackage[utf8]{inputenc}" << endl;
    fout << "\\begin{document}" << endl;
    fout << "\\thispagestyle{empty}" << endl;
    fout << endl;

    for (string trigger : triggers) {

        cout << "Working on table for trigger " << trigger << endl;

        //--- get yields
        vector<vector<float>> yields;
        for (string file_name : file_names) {

            //--- load file
            string ntuple_path = "/eos/atlas/atlascerngroupdisk/phys-susy/2L2J-ANA-SUSY-2018-05/SusySkim2LJets/v1.6/SUSY2/";
            string full_filename = ntuple_path + file_name + "_merged_processed.root";
            cout << "File " << file_name << endl;
            TChain* tch;
            if (file_name.find("data") != string::npos) tch = new TChain("data");
            else tch = new TChain((file_name.substr(file_name.find("/")+1) + "_NoSys").c_str());
            tch->Add(full_filename.c_str());

            vector<float> file_yields;
            for (string flavor : flavors) {
                int current_yield = tch->GetEntries((trigger + " && " + flavor).c_str());
                //cout << current_yield << endl;
                file_yields.push_back(current_yield);
            }
            yields.push_back(file_yields);
            delete tch;
        }

        //--- fill table
        fout << "\\begin{table}[]" << endl;
        fout << "\\begin{center}" << endl;
        boost::replace_all(trigger, "_", "\\_");
        if (trigger == "1") fout << "Inclusive Counts" << endl;
        else fout << trigger << " Counts" << endl;
        fout << "\\begin{tabular}{l|l|l|l}" << endl;
        fout << "& ee & mm & em \\\\" << endl;
        fout << "\\hline" << endl;
        for (int i=0; i<file_names.size(); i++) {
            fout << file_labels[i] << " & " << yields[i][0] << " & " << yields[i][1] << " & " << yields[i][2] << " \\\\" << endl;
        }
        fout << "\\end{tabular}" << endl;
        fout << "\\end{center}" << endl;
        fout << "\\end{table}" << endl;
        fout << endl;
    }

    //--- finish generating LaTeX
    fout << "\\end{document}" << endl;
    system("latex table.tex");
    system("dvipdf table.dvi");
}
