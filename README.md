# Golf Ball Launcher

This was a project that I undertook as part of my curriculum at the University of California, Irvine. The proejct revolved around designing and developing a golf ball launcher capable of launching a golf ball into a bucket at a specific distance away. As part of a six-person team, we worked together to follow the engineering design process and ensure the design we had was the best possible scenario for our given situation. For this project, I was the programming lead, taking on the challenge of designing the dynamic model for the launcher as well as designing some of the major components for the project. 

## Objective
The objective of the project was to design a golf ball launcher that could launch a golf ball directly into a bucket given a random platform height and bucket distance, as well as launching the golf ball with at least one bounce into the bucket. 

## Requirements
<li>
  System must be able to determine optimal launch angle and change launch angle based on given input distance
</li>
<li>
  System must be able to launch the golf ball directly into the bucket
</li>
<li>
  System must be able to launch the golf ball into the bucket with at least one bounce
</li>
<li>
  System must be able to measure the exit velocity of the golf ball
</li>
<li>
  Must conform to budget ($110 for parts, $65 for manufacturing)
</li>
  
## The Golf-Ball Launcher
Down below is the final SOLIDWORKS model for the launcher. 
<p align="center">
  <img width="560" height="460" src="https://github.com/varmagokul6/Golf-Ball-Launcher/blob/main/images/golflaunch_cad.png">
</p>




## Dynamic Model
The dynamic model is a method to describe the trajectory of the golf ball in the two required scenarios. The script takes in an input distance and platform height, and 
runs calculations for the optimal output angle for the golf ball, as shown in the figure below. This script was also used to determine the spring constant for the
launching mechanism.
<p align="center">
  <img width="560" height="460" src="https://github.com/varmagokul6/Golf-Ball-Launcher/blob/main/images/dynamic_model.png">
</p>

## Video Demonstrations

Distance: 10 ft, Platform Height: 48 in, Direct Shot

https://user-images.githubusercontent.com/44277090/174935235-e9701bfd-6994-4125-adef-8f9ce802d89e.mp4



Distance: 20 ft, Platform Height: 48 in, Minimum One Bounce

https://user-images.githubusercontent.com/44277090/174935278-6f05ea36-9b93-4060-be90-fc65f4e44161.mp4





