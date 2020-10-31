#!/bin/sh

while read line ; do
    echo $line | awk '{print $1":"$8":"$10}' 
done < <(grep bufsize stats_Os.log) > stats.dat

(cat > ./stats.C)<<EOF
{
    TFile * f = new TFile("stats_Os.root", "recreate");
    TTree * t = new TTree("stats","stats"); 
    t->ReadFile("stats.dat", "Op/C:Rate/F:Size/F", ':'); 
    t->Print();
    t->Write();
    f->Close();
}
EOF

root -l -q -b ./stats.C
rm stats.dat;
rm stats.C;
