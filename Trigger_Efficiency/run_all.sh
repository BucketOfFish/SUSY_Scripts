#for feature in "Ptll" "met_Et" "Ht30" "nJet30"
for feature in "mll" "lepPt[0]" "lepPt[1]"
do
    for period in "SUSY2_Signal_mc16a" "SUSY2_Signal_mc16cd" "SUSY2_Signal_mc16e"
    do
        for process in "C1N2_WZ_600_0_2L2J" "C1N2_WZ_200p0_100p0_2L2J"
        do
            root -l -b -q 'TriggerEfficiency.C("'${period}'", "'${process}'", "'${feature}'")'
        done
    done

    for period in "data15-16" "data17" "data18"
    do
        root -l -b -q 'TriggerEfficiency.C("SUSY2_Data", "'${period}'", "'${feature}'")'
    done
done
