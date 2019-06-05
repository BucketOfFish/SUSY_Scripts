#include "../CommonLibraries.C"

using namespace std;

void SUSYTriggers_Table() {

    vector<string> file_names {"SUSY2_Data/data15-16", "SUSY2_Data/data17", "SUSY2_Data/data18", "SUSY2_Signal_mc16a/C1N2_WZ_600_0_2L2J", "SUSY2_Signal_mc16cd/C1N2_WZ_600_0_2L2J", "SUSY2_Signal_mc16e/C1N2_WZ_600_0_2L2J", "SUSY2_Signal_mc16a/C1N2_WZ_200p0_100p0_2L2J", "SUSY2_Signal_mc16a/C1N2_WZ_200p0_100p0_2L2J", "SUSY2_Signal_mc16a/C1N2_WZ_200p0_100p0_2L2J"};
    vector<string> file_labels {"data15-16", "data17", "data18", "(600, 0) MC16a", "(600, 0) MC16cd", "(600, 0) MC16e", "(200, 100) MC16a", "(200, 100) MC16cd", "(200, 100) MC16e"};
    vector<string> triggers {"trigMatch_1L2LTrig", "trigMatch_1L2LTrigOR", "trigMatch_2LTrigOR"};
    vector<string> flavors {"lepFlavor[0]==1 && lepFlavor[1]==1", "lepFlavor[0]==2 && lepFlavor[1]==2", "((lepFlavor[0]==2 && lepFlavor[1]==1) || (lepFlavor[0]==1 && lepFlavor[1]==2))"};
    vector<vector<float>> yields;

    for (string trigger : triggers) {
        for (string file_name : file_names) {

            //--- load file
            string ntuple_path = "/eos/atlas/atlascerngroupdisk/phys-susy/2L2J-ANA-SUSY-2018-05/SusySkim2LJets/v1.6/SUSY2/";
            string full_filename = ntuple_path + file_name + "_merged_processed.root";
            cout << "Working on file " << file_name << endl;
            TChain* tch;
            if (file_name.find("data") != string::npos) tch = new TChain("data");
            else tch = new TChain((file_name.substr(file_name.find("/")+1) + "_NoSys").c_str());
            tch->Add(full_filename.c_str());

            //--- get yields
            vector<float> file_yields;
            for (string flavor : flavors) {
                file_yields.push_back(tch->GetEntries((trigger + " && " + flavor).c_str()));
            }
            yields.push_back(file_yields);

            delete tch;
        }

        //--- make LaTeX table
        ofstream fout("Tables/table_" + trigger + ".txt", ios::out);
        fout << "\\begin{table}[]" << endl;
        fout << "\\begin{tabular}{@{}|l|l|l|l|@{}}" << endl;
        fout << "\\toprule" << endl;
        fout << "                  & ee & mm & em \\\\ \\midrule" << endl;
        for (int i=0; i<file_names.size(); i++) {
            fout << file_labels[i] << " & " << yields[i][0] << " & " << yields[i][1] << " & " << yields[i][2] << " \\\\ \\midrule" << endl;
            //fout << "(200, 100) MC16e  & " << yields[0][0] << " & " << yields[0][0] << " & " << yields[0][0] << " \\\\ \\bottomrule" << endl;
        }
        fout << "\\end{tabular}" << endl;
        fout << "\\end{table}" << endl;
    }
}
