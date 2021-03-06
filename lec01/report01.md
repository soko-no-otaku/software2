## 課題4.の説明

### 拡張したルール

- 各セルの状態として、生・死の2段階から死・子供・大人の3段階に拡張した．
- 死んだセルからは8近傍に大人が2セルあるときに限り子供が誕生する．
- 子供のセルは，8近傍に子供が少なくとも1セルいて，かつ大人が2or3セルいれば成長する．
- 上の条件で子供がいない場合には成長せずそのままで，それ以外では死滅する．
- 大人のセルは，8近傍に子供が少なくとも1セルいれば生きながらえる．

### 結果

- 初期配置は各状態の出現確率を等しくした．
- 最初から数世代後には大人・子供各4セル程度のグループ（家族，と呼ぶことにする）が点在し，長世代に渡り生き続けた．
- "家族"同士がぶつかると，一度大きな塊になった後小さな"家族"に分散した．
- 全体の傾向としては"家族"単位で生き残っているように観察できた．
- オリジナルのライフゲームでの oscillator に相当するものも確認できた．
- 何世代経過しても同じ形を保ち続けるような配置も見られた．
