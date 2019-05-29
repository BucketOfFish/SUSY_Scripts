root -l /eos/atlas/atlascerngroupdisk/phys-susy/2L2J-ANA-SUSY-2018-05/SusySkim2LJets/v1.6/SUSY2/SUSY2_Data/data15-16_merged_processed.root
data->Scan("lepFlavor:lepPt:RunNumber", "trigMatch_1L2LTrig!=trigMatch_1L2LTrigOR")
make comparison plots of RunNumbers that pass 1L2L vs. 1L2LOR for ee, mm, em
