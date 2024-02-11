
# ⭐ About this project
This project was implemented to simulate a proton experiment at HUS.
## 1. Geometry

## 2. Physics

## 3. Primary source
* The primary electron beam was created by using `G4ParticleGun` class.
* There are availabled commands to setups beam:

|       **type**      	| **commands**                      	| **value**   	| **units** 	| **description**                                      	|
|:-------------------:	|-----------------------------------	|-------------	|-----------	|------------------------------------------------------	|
| energy distribution 	| /LINAC/Beam/SetBeamEnergy         	| mean_energy 	| energy    	| Thiết lập năng lượng trung bình cho phân bố Gaussian 	|
|                     	| /LINAC/Beam/SetEnergyStd          	| sigma       	| energy    	| Thiết lập giá trị sigma cho phân bố Gaussian         	|
| geometry            	| /LINAC/Beam/SetBeamRadius         	| r           	| length    	| Thiết lập bán kính chùm electron                     	|
|                     	| /LINAC/Beam/SetBeamXPos           	| x           	| length    	| Thiết lập tọa độ X của nguồn                         	|
|                     	| /LINAC/Beam/SetBeamYPos           	| y           	| length    	| Thiết lập tọa độ Y của nguồn                         	|
|                     	| /LINAC/Beam/SetBeamZPos           	| z           	| length    	| Thiết lập tọa độ Z của nguồn                         	|
| angle distribution  	| /LINAC/Beam/SetMinSolidAngle      	| min_angle   	| angle     	| Thiết lập giới hạn dưới góc phát hạt                 	|
|                     	| /LINAC/Beam/SetMaxSolidAngle      	| max_angle   	| angle     	| Thiết lập giới hạn trên góc phát hạt                 	|
| direction           	| /LINAC/Beam/SetThetaRotationAngle 	| theta       	| angle     	| Thiết lập góc quay theta                             	|
|                     	| /LINAC/Beam/SetPhiRotationAngle   	| phi         	| angle     	| Thiết lập góc quay phi                               	|

## 4. Scoring
* This application used `G4MultiFunctionalDetector` and `G4VPrimitiveScorer` classes to score the absorbed dose in the water phantom volume.
* The results will be exported into text file.
* The results cannot show uncertainty of the dose.

# 🔧 Requirements
  * Ubuntu 20.04/Window 10
  * Geant4.11.1.2
  * Root data analysis framework

# 🏃‍♂️ How to run
* Run example in interactive mode
    ```c++
    ./sim
    ```
* Run example in batch mode
    ```c++
    ./sim run.mac
    ```
# 🚀 About Me
**Bùi Tiến Hưng** - *hungbt1908@gmail.com*
1. Nuclear Engineering Lab, Hanoi University of Science and Technology (HUST).
2. Vietnam Atomic Energy Institute (VINATOM).

**Interestet Fields:**
* Medical physics, 
* Monte-Carlo simulation, 
* Machine learning.