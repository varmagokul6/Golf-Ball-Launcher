clear
clc

% 1. Physical Parameters
y = 24;             % Platform Height [in]
h = 14.5;           % Bucket Height [in]
w = 13;             % Bucket Width [in]
b_dist = 5;         % Bucket Distance [ft]
%d2 = 0;            % Second Target Distance [ft]
g = 9.8;            % Acceleration Due to Gravity [m/s^2]
m = .0459;          % Mass of Golf Ball [kg]

% 2. Golf Ball Launcher Settings
k = 9;             % Spring Constant [lbs/in]
l = 5;             % Length of Launch Canister [in]
d1 = 1;            % Scenario 1 Spring Compression Length [in]
d2 = 1;            % Scenario 2 Spring Compression Length [in]
theta1 = 25;       % Scenario 1 Launch Angle [degrees]
theta2 = 55;       % Scenario 2 Launch Angle [degrees] 
loss = .1;         % Percentage of energy lost due to friction

dt = .01;
curve1 = [];
curve2 = [];


% 3. Scenario 1 Trajectory Simulation

% 3a. Conversions
y = in2m(y);
h = in2m(h);
w = in2m(w);
k = k * 175.126835;     % lb/in to N/m
d1 = in2m(d1);
b_dist = ft2m(b_dist);

% 3b. Scenario 1 Calculations
% 3b1. Energy Balance
for theta = 0:1:theta1
    PE1_1 = .5 * k * d1^2 + m*g*(l-d1)*sind(theta);    % Initial Potential Energy
    PE2_1 = m*g*l*sind(theta);                         % Final Potential Energy
    vi_1 = sqrt(2*(PE1_1 - PE2_1)/m);
    %fprintf("Exit Velocity: %4.4f m/s or %4.4f ft/s\n", vi_1, vi_1 * 3.28084)
    
    % 3b2. Scenario 1 Projectile Motion
    viy_1 = vi_1 * sind(theta);
    vix_1 = vi_1 * cosd(theta);
    trajectory_1 = [-.5*g viy_1 y];
    t_1 = roots(trajectory_1);
    for i = 1:length(t_1)
        if t_1(i) > 0
            t_1 = t_1(i);
            break
        end
    end
    dist_trav = vix_1 * t_1; 
    entrance_time = b_dist/vix_1;
    y_entry = y + viy_1*entrance_time - .5*g*entrance_time^2;
    
    if ((dist_trav <= (b_dist + w)) && ((dist_trav >= b_dist)) && (y_entry > h))
        curve1 = [];
        fprintf("Time to Reach Bucket: %4.4f seconds\n", t_1)
        fprintf("Theta: %4.4f, Distance Traveled: %4.4f, Entrance Height: %4.4f (X: %4.4f - %4.4f, Y: %4.4f)\n", theta, dist_trav, y_entry, b_dist, b_dist + w, h)
        for i = 0:dt:t_1
            x_coord1 = m2in(vix_1*i);
            y_coord1 = m2in(y + viy_1*i - .5*g*i^2);
            curve1 = [curve1; x_coord1, y_coord1];
        end
        bucket_pos_1 = [m2in(b_dist) 0 m2in(w) m2in(h)];
        launcher_pos = [0 0 .5 m2in(y)];
        rectangle('Position', bucket_pos_1, 'EdgeColor', 'r')
        %rectangle('Position', launcher_pos, 'EdgeColor', 'r')
        xlim([0 100]);
        ylim([0 30])
        hold on
        plot(curve1(:, 1), curve1(:, 2));
        hold on
    end
    % 3c. Trajectory Visualization    
end




function output = in2m(inches)
output = inches * 0.0254;
end

function output = ft2m(feet)
output = feet * 0.3048;
end

function output = m2in(m)
output = m/0.0254;
end

function output = m2ft(m)
output = m/.3048;
end