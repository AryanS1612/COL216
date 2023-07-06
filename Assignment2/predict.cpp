#include "./BranchPredictor.hpp"
#include<bits/stdc++.h>
using namespace std;

uint32_t hextodeci(string s){
    uint32_t ans = 0;
    uint32_t val = 1;
    int n = s.size();int i= n-1;
    while(i >= 0){
        (int(s[i]) >= 97) ? ans += val*(int(s[i]) - 87) : ans += val*(int(s[i]) - 48);
        val *= 16;
        i--;
    }
    return ans;
}

int main(){
    int a;
    cout << "Input the initializing value" << endl;
    cin >> a;
    SaturatingBranchPredictor SaturationPredi(a);
    BHRBranchPredictor BHRPredi(a);
    SaturatingBHRBranchPredictor SatBHRPredi(a, 1 << 16);
    int n;
    cin >> n;
    float SaturationCorrect = 0;
    float BHRCorrect = 0;
    float SatBHRCorrect = 0;
    float totalBranches = float(n);
    for(int i = 0;i < n;i++){
        string addr; bool taken;
        cin >> addr >> taken;
        uint32_t pc = hextodeci(addr);
        if(SaturationPredi.predict(pc) == taken) SaturationCorrect++;
        SaturationPredi.update(pc, taken);
        if(BHRPredi.predict(pc) == taken) BHRCorrect++; //cout << BHRPredi.predict(pc) << endl;
        BHRPredi.update(pc, taken);
        if(SatBHRPredi.predict(pc) == taken) SatBHRCorrect++;
        SatBHRPredi.update(pc, taken);
    }
    // cout << BHRCorrect << endl;
    cout << "Accuracy of Saturation Branch Predictor with Branch History is : " << SatBHRCorrect/totalBranches << endl;
    cout << "Accuracy of Saturation Branch Predictor is : " << SaturationCorrect/totalBranches << endl;
    cout << "Accuracy of BHR Branch Predictor is : " << BHRCorrect/totalBranches << endl;
}