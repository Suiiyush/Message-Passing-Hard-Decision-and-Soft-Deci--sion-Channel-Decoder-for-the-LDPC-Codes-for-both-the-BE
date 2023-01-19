Channel coding aims at making channel noiseless mostly accomplished by introducing redundant bits in the information stream.

This is a Message Passing Hard Decision and Soft Decision Channel Decoder for the LDPC Codes for both the Binary erasure (BEC) and the Binary symmetric (BSC) channels.

It includes a model of BEC with erasure probability p and BSC with bit flip probability p.

The code has been given three parity check H matrices in order to compare the performance across both channels for different algorithms. It is assumed that an all-zero codeword has been transmitted. The code automatically converts the patterns of 1's and 0's in the parity check matrix H into a Tanner Graph model. The connections between the bit nodes and the check nodes of this bipartite graph are based on the location of ones in H. We then take an all zero codeword and pass it through our BSC or BEC which introduces errors in the bits. The interative message passing decoding will be done on this errornous codeword.

The model of Tanner Graph performs the message passing from bit node to the variable nodes and vice-versa. Each check node is an SPC code. It enforces the even parity among all the bit nodes that are connected to it. Thus message passed from variable node to check node is based on the SPC decoding. Similarly, each bit node is essentially a repetition code. It requires that all the messages are the same (0 or 1). Thus the message passed from VN to CN is based on the majority vote decoding of the repetition codes.

We performed Monte Carlo simulations for each value of p for the BEC and BSC and for each of the three LDPC codes (H matrices), we perform around 10000 simulations.

Comparision of Hard decoding for BEC:
![image-055](https://user-images.githubusercontent.com/73515280/213473578-11865097-a3bc-4a61-a5d4-a6a2145120ed.png)

