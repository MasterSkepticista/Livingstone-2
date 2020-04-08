# Livingstone-2
Repackaged NASA Livingstone-2 Oliver and L2 engine in Java and C++.

> If you're here, you must know what this package is all about. If you're building it, this repo should be very helpful.

Links to get you started:

Livingstone 2 (System for Automated Diagnosis and Discrete Control of Complex Systems) and Skunkworks (Suite of Supporting Development and Runtime Tools): https://software.nasa.gov/software/ARC-14725-1

Source repo: http://ti.arc.nasa.gov/opensource/projects/livingstone2/

Note: jdk has to be installed manually. Simply extract jdk folder to the root of L2. I used jdk1.6.x

## Installing Livingstone 2 and Oliver Package(s)

You only need to add one line to the PATH of System Environment Variables

On Windows 10,
1. Press Start -> Search, type "Edit the System Environment Variables"
2. Press "Environment Variables"
3. In the second section "System Paths", find an entry called PATH
4. Click Edit -> New
5. Now paste this path: `%ThisRootDirectory%\mba\cpp\lib`

Replace `%ThisRootDirectory%` with the path where you opened this, for example, PATH should have 
entry SIMILAR to this:

 `D:\MyFavoriteFolder\L2Root_pack\mba\cpp\lib`


## Using Oliver (Stanley-II)

1. File -> Open -> Navigate to `%here%\L2Root\l2-tools\groundworks\examples`
	OR
1. You could make your own model, from the documentation given in the official
L2 package by NASA.

2. After it is ready, right-click on the main component on left column, click
"Make Root Component"

3. Right click again on the main component and press "Compile"
4. Right click again and press RUN.


## Scenario Manager

Will open.

Open a scenario file from the File -> Open Scenario.

These files have extension .scr and can be made in accordance to the docs.

Press STEP/RUN/SINGLE depending on how many instructions you want to execute at a time.

## Troubleshooting

Most likely, it closed without any error trace.

Only thing to go wrong here, is incorrect Environment Variable setting.

Did you choose the second section "System Paths" in step 2.? 

Don't choose "User Paths"

The package is standalone, it should work without any dependencies (except for the JDK which I assume you read earlier).

Still doesn't work? Open an issue.
I'll be glad to get back to your problems.

Please feel free to cite this work if it helped you anyway.
