Before I started the project I had thought that it's all about kinematics and I'll finish the project in one day.
After a couple of months I realised that making the program working properly requires implementing quanta of energy like in modern physics (I didn't know about issues with floating-point numbers in programming)

## Dictionary:
HP - hit points; 
	 the greater the number the more energy a specific animal has,
	 the smaller the number the closer an animal is death

BMR - basal metabolic rate;
	  metabolic cost, how many hit points a specific animal loses for sake of surviving,
	  no matter its acceleration and velocity

## Ecosystem rules:
I'm going to gradually add here the rules of the ecosystem:

* cloning:
  its up to animals if they want to clone 
  (if 3rd output >= 0 then an animal wants to clone, if its < 0 then it don't want to clone);

  but there are some requirements to make cloning possible 
  even if an animal wants to do it:
  + a specific animal can clone itself only once every 5 seconds
  + its HP has to be > 1, because HP is always an integer number; 1 cannot be divided in half;
  	but HP is hardly ever = 1, its almost always a greater number;
  	it can reach values like 10'000 or 1'000'000 or even greater numbers
  	(it depends on initial ecosystem settings)
  + it cannot be close to borders 
  	(it can be at most in 90% of the distance from the center of the arena to the borders)

## User features:
I'm going to gradually add here the features of the project:

* by holding +/- you are able to zoom in/out the view of the ecosystem camera
* by holding Z key you are able to zoom out the camera so that you can see what is beyond the world