#include "../CommonLibraries.C"
#include <boost/algorithm/string/replace.hpp>

using namespace std;

void SUSYTriggers_Table() {

    string ntuple_path = "/eos/atlas/atlascerngroupdisk/phys-susy/2L2J-ANA-SUSY-2018-05/SusySkim2LJets/v1.7/SUSY2/";
    vector<string> file_names {"SUSY2_Data/data15-16", "SUSY2_Data/data17", "SUSY2_Data/data18", "SUSY2_Signal_mc16a/C1N2_WZ_600_0_2L2J", "SUSY2_Signal_mc16cd/C1N2_WZ_600_0_2L2J", "SUSY2_Signal_mc16e/C1N2_WZ_600_0_2L2J", "SUSY2_Signal_mc16a/C1N2_WZ_200p0_100p0_2L2J", "SUSY2_Signal_mc16cd/C1N2_WZ_200p0_100p0_2L2J", "SUSY2_Signal_mc16e/C1N2_WZ_200p0_100p0_2L2J"};
    vector<string> file_labels {"data15-16", "data17", "data18", "(600, 0) MC16a", "(600, 0) MC16cd", "(600, 0) MC16e", "(200, 100) MC16a", "(200, 100) MC16cd", "(200, 100) MC16e"};
    vector<string> regions = {"1", "(nLep_signal==2 && nJet30>=2 && met_Et>200)", "mt2leplsp_0>70 && mll<50 && met_Et>250 && Ht30>200 && minDPhi2JetsMet>0.4 && nJet30>=2 && lepFlavor[0]==lepFlavor[1] && met_Sign>8", "mt2leplsp_0>50 && met_Et>400 && Ht30>400 && minDPhi2JetsMet>0.4 && nJet30>=2 && lepFlavor[0]==lepFlavor[1]", "mt2leplsp_0>100 && met_Et>200 && Ht30>1200 && minDPhi2JetsMet>0.4 && nJet30>=2 && lepFlavor[0]==lepFlavor[1]"};
    vector<string> region_names = {"Inclusive", "Baseline", "SR Low", "SR Medium", "SR High"};
    vector<string> triggers {"1", "trigMatch_1L2LTrig", "trigMatch_1L2LTrigOR", "trigMatch_2LTrig", "trigMatch_2LTrigOR"};
    vector<string> flavors {"lepFlavor[0]==1 && lepFlavor[1]==1", "lepFlavor[0]==2 && lepFlavor[1]==2", "((lepFlavor[0]==2 && lepFlavor[1]==1) || (lepFlavor[0]==1 && lepFlavor[1]==2))"};

    //--- get yields
    vector<vector<vector<vector<int>>>> yields; // [file][region][trigger][flavor]
    for (string file_name : file_names) {

        //--- load file
        cout << "Working on trigger table for file " << file_name << endl;
        string full_filename = ntuple_path + file_name + "_merged_processed.root";
        TChain* tch;
        if (file_name.find("data") != string::npos) tch = new TChain("data");
        else tch = new TChain((file_name.substr(file_name.find("/")+1) + "_NoSys").c_str());
        tch->Add(full_filename.c_str());

        vector<vector<vector<int>>> file_yields;
        for (string region : regions) {
            cout << "\t" << region << endl;
            vector<vector<int>> region_yields;
            for (string trigger : triggers) {
                cout << "\t\t" << trigger << endl;
                vector<int> trigger_yields;
                for (string flavor : flavors) {
                    trigger_yields.push_back(tch->GetEntries((region + " && " + trigger + " && " + flavor).c_str()));
                }
                region_yields.push_back(trigger_yields);
            }
            file_yields.push_back(region_yields);
        }
        delete tch;
        yields.push_back(file_yields);
    }

    //--- make LaTeX table
    ofstream fout("table.tex", ios::out);
    fout << "\\documentclass{article}" << endl;
    fout << "\\usepackage[utf8]{inputenc}" << endl;
    fout << "\\usepackage{amsmath}" << endl;
    fout << "\\begin{document}" << endl;
    fout << "\\thispagestyle{empty}" << endl;
    fout << endl;

    //--- yield tables
    for (int r=0; r<regions.size(); r++) {
        string region_name = region_names[r];
        for (int t=0; t<triggers.size(); t++) {
            string trigger = triggers[t];
            boost::replace_all(trigger, "_", "\\_");
            boost::replace_all(trigger, "&", "\\&");
            fout << "\\begin{center}" << endl;
            if (t == 0) fout << "Inclusive Counts in " << region_name << " Region \\\\" << endl;
            else fout << trigger << " Counts in " << region_name << " Region \\\\" << endl;
            fout << "\\begin{tabular}{l|l|l|l}" << endl;
            fout << "& ee & mm & em \\\\" << endl;
            fout << "\\hline" << endl;
            for (int f=0; f<file_names.size(); f++)
                fout << file_labels[f] << " & " << yields[f][r][t][0] << " & " << yields[f][r][t][1] << " & " << yields[f][r][t][2] << " \\\\" << endl;
            fout << "\\end{tabular}" << endl;
            fout << "\\end{center}" << endl;
            fout << endl;
        }
        fout << "\\newpage" << endl;
        fout << endl;
    }


    //--- efficiency tables
    cout << setprecision(2) << fixed;
    for (int r=1; r<regions.size(); r++) {
        string region_name = region_names[r];
        for (int t=1; t<triggers.size(); t++) {
            string trigger = triggers[t];
            boost::replace_all(trigger, "_", "\\_");
            boost::replace_all(trigger, "&", "\\&");
            fout << "\\begin{center}" << endl;
            fout << "\\text{" << trigger << " Efficiency in " << region_name << " Region (\\\%)} \\\\" << endl;
            fout << "\\begin{tabular}{l|l|l|l}" << endl;
            fout << "& ee & mm & em \\\\" << endl;
            fout << "\\hline" << endl;
            for (int f=0; f<file_names.size(); f++)
                fout << file_labels[f] << " & " << (float)yields[f][r][t][0]/yields[f][r][0][0]*100 << " & " << (float)yields[f][r][t][1]/yields[f][r][0][1]*100 << " & " << (float)yields[f][r][t][2]/yields[f][r][0][2]*100 << " \\\\" << endl;
            fout << "\\end{tabular}" << endl;
            fout << "\\end{center}" << endl;
            fout << endl;
        }
        fout << "\\newpage" << endl;
        fout << endl;
    }

    //--- finish generating LaTeX
    fout << "\\end{document}" << endl;
    system("latex table.tex");
    system("dvipdf table.dvi");
}
