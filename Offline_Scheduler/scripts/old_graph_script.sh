# #!/bin/bash
# rflag=false
# small_r=false
# big_r=false
# 
# # FIX 
# usage () { echo "How to use"; }

# if [ -z "$1" ] ; then 
#     echo "Incorrect usage of $0."
#     echo "Enter the name of the directory to graph as the first command line argument"
#     exit
# fi
# 
# echo "Choose a parameter to graph"
# select opt in "Mutation" "Crossover" "Quit"; do
#     if [ "$opt" = "Quit" ]; then 
#         exit
#     elif [ "$opt" = "Mutation" ] || [ "$opt" = "Crossover" ]; then
#         break;
#     fi
# done
# 
# echo "Choose a fitness to graph?"
# select opt in "Average" "Minimum" "Both" "Quit"; do
#     if [ "$opt" = "Quit" ]; then 
#         exit
#     elif [ "$opt" = "Average" ] || [ "$opt" = "Minimum" ] || [ "$opt" = "Both" ]; then
#         break;
#     fi
# done




# options=':ij:rRvh'
# while getopts $options option
# do
#     case $option in
#         i  ) i_func;;
#         j  ) j_arg=$OPTARG;;
#         r  ) rflag=true; small_r=true;;
#         R  ) rflag=true; big_r=true;;
#         v  ) v_func; other_func;;
#         h  ) usage; exit;;      
#         \? ) echo "Unknown option: -$OPTARG" >&2; exit 1;;
#         :  ) echo "Missing option argument for -$OPTARG" >&2; exit 1;;
#         *  ) echo "Unimplimented option: -$OPTARG" >&2; exit 1;;
#     esac
# done
# 
# shift $(($OPTIND - 1))
# 
# if ! $rflag && [[ -d $1 ]]
# then
#     echo "-r or -R must be included when a directory is specified" >&2
#     exit 1
# fi
# 
# for file in /{,usr/}bin/*calc

