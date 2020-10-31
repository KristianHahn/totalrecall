#!/bin/sh

if [ "$#" -ne 1 ]; then
    echo "Error :: arguments: <filename>.log"
    return 1;
fi;

rootfile=`echo $1 |  sed -e's,.log,.root,'`

while read line ; do
    echo $line | awk '{print $1":"$8":"$10}' 
done < <(grep bufsize $1) > stats.dat

(cat > ./stats.C)<<EOF
{
    TFile * f = new TFile("$rootfile", "recreate");
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
