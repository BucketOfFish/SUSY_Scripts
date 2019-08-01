#include "../CommonLibraries.C"
#include <boost/algorithm/string/replace.hpp>

using namespace std;

void SUSYTriggers_Table() {

    string ntuple_path = "/eos/atlas/atlascerngroupdisk/phys-susy/2L2J-ANA-SUSY-2018-05/SusySkim2LJets/v1.6/SUSY2/";
    vector<string> file_names {"SUSY2_Data/data15-16", "SUSY2_Data/data17", "SUSY2_Data/data18", "SUSY2_Signal_mc16a/C1N2_WZ_600_0_2L2J", "SUSY2_Signal_mc16cd/C1N2_WZ_600_0_2L2J", "SUSY2_Signal_mc16e/C1N2_WZ_600_0_2L2J", "SUSY2_Signal_mc16a/C1N2_WZ_200p0_100p0_2L2J", "SUSY2_Signal_mc16cd/C1N2_WZ_200p0_100p0_2L2J", "SUSY2_Signal_mc16e/C1N2_WZ_200p0_100p0_2L2J"};
    vector<string> file_labels {"data15-16", "data17", "data18", "(600, 0) MC16a", "(600, 0) MC16cd", "(600, 0) MC16e", "(200, 100) MC16a", "(200, 100) MC16cd", "(200, 100) MC16e"};
    string baseline = "(nLep_signal==2 && nJet30>=2 && met_Et>200)";
    vector<string> triggers {"1", "trigMatch_1L2LTrig", "trigMatch_1L2LTrigOR", "trigMatch_2LTrig", "trigMatch_2LTrigOR"};
    vector<string> flavors {"lepFlavor[0]==1 && lepFlavor[1]==1", "lepFlavor[0]==2 && lepFlavor[1]==2", "((lepFlavor[0]==2 && lepFlavor[1]==1) || (lepFlavor[0]==1 && lepFlavor[1]==2))"};

    //--- get yields
    vector<vector<vector<int>>> yields; // [file][trigger][flavor]
    vector<vector<vector<int>>> baseline_yields; // additional baseline selection
    for (string file_name : file_names) {

        //--- load file
        cout << "Working on trigger table for file " << file_name << endl;
        string full_filename = ntuple_path + file_name + "_merged_processed.root";
        TChain* tch;
        if (file_name.find("data") != string::npos) tch = new TChain("data");
        else tch = new TChain((file_name.substr(file_name.find("/")+1) + "_NoSys").c_str());
        tch->Add(full_filename.c_str());

        vector<vector<int>> file_yields, file_baseline_yields;
        for (string trigger : triggers) {

            cout << trigger << endl;
            vector<int> trigger_yields, trigger_baseline_yields;
            for (string flavor : flavors) {
                trigger_yields.push_back(tch->GetEntries((trigger + " && " + flavor).c_str()));
                trigger_baseline_yields.push_back(tch->GetEntries((baseline + " && " + trigger + " && " + flavor).c_str()));
            }
            file_yields.push_back(trigger_yields);
            file_baseline_yields.push_back(trigger_baseline_yields);
        }
        delete tch;
        yields.push_back(file_yields);
        baseline_yields.push_back(file_baseline_yields);
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
    for (int j=0; j<triggers.size(); j++) {
        string trigger = triggers[j];
        boost::replace_all(trigger, "_", "\\_");
        boost::replace_all(trigger, "&", "\\&");
        fout << "\\begin{table}[]" << endl;
        fout << "\\begin{center}" << endl;
        if (j == 0) fout << "Inclusive Counts \\\\" << endl;
        else fout << trigger << " Counts \\\\" << endl;
        fout << "\\begin{tabular}{l|l|l|l}" << endl;
        fout << "& ee & mm & em \\\\" << endl;
        fout << "\\hline" << endl;
        for (int i=0; i<file_names.size(); i++)
            fout << file_labels[i] << " & " << yields[i][j][0] << " & " << yields[i][j][1] << " & " << yields[i][j][2] << " \\\\" << endl;
        fout << "\\end{tabular}" << endl;
        fout << "\\end{center}" << endl;
        fout << "\\end{table}" << endl;
        fout << endl;
    }

    //--- efficiency tables
    cout << setprecision(2) << fixed;
    for (int j=0; j<triggers.size(); j++) {
        string trigger = triggers[j];
        boost::replace_all(trigger, "_", "\\_");
        boost::replace_all(trigger, "&", "\\&");
        if (j == 0) continue;
        fout << "\\begin{table}[]" << endl;
        fout << "\\begin{center}" << endl;
        fout << "\\text{" << trigger << " Efficiency Over Baseline (\\\%)} \\\\" << endl;
        fout << "\\begin{tabular}{l|l|l|l}" << endl;
        fout << "& ee & mm & em \\\\" << endl;
        fout << "\\hline" << endl;
        for (int i=0; i<file_names.size(); i++)
            fout << file_labels[i] << " & " << (float)baseline_yields[i][j][0]/baseline_yields[i][0][0]*100 << " & " << (float)baseline_yields[i][j][1]/baseline_yields[i][0][1]*100 << " & " << (float)baseline_yields[i][j][2]/baseline_yields[i][0][2]*100 << " \\\\" << endl;
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
