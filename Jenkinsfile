properties(
	[
		buildDiscarder(logRotator(artifactDaysToKeepStr: '', artifactNumToKeepStr: '', daysToKeepStr: '', numToKeepStr: '10')),
		pipelineTriggers([pollSCM('0 H(5-6) * * *')])
	]
)

pipeline
{
	agent none
	
	stages {
		stage('Build native') {
			parallel {
				/*
				stage( 'Build autoconf gcc') {
					agent {
						node {
							label 'linuxarmhf && development'
						}
                    }
					steps {
						sh '''
							cd autoconf
							autoreconf --install --force
							./configure
							make -j4
						'''
						cleanWs()
					}			
				}
				stage( 'Build CMake gcc') {
					agent {
						node {
							label 'linuxarmhf && development'
						}
                    }
					steps {
						sh '''
							cd CMake
							cmake .
							make -j4
							cp ./eLinkDisplayStatus ./eLinkDisplayStatus-linuxarmhf-gcc
						'''
						archiveArtifacts artifacts: 'CMake/eLinkDisplayStatus-linuxarmhf-gcc', onlyIfSuccessful: true
						cleanWs()
					}			
				}
				stage( 'Build CMake llvm') {
					agent {
						node {
							label 'linuxarmhf && development'
						}
                    }
					steps {
						sh '''
							cd CMake
							cmake -DCMAKE_TOOLCHAIN_FILE=llvm.cmake
							make -j4
							cp ./eLinkDisplayStatus ./eLinkDisplayStatus-linuxarmhf-llvm
						'''
						archiveArtifacts artifacts: 'CMake/eLinkDisplayStatus-linuxarmhf-llvm', onlyIfSuccessful: true
						cleanWs()
					}			
				}
				*/
				stage( 'Build CMake cross-compilation gcc armhf') {
					agent {
						node {
							label 'linux && development'
						}
                    }
					steps {
						sh '''
							cd CMake
							cmake -DCMAKE_TOOLCHAIN_FILE=arm-linux-gnueabihf.cmake
							make -j4
							cp ./eLinkDisplayStatus ./eLinkDisplayStatus-arm-linux-gnueabihf-gcc
						'''
						archiveArtifacts artifacts: 'CMake/eLinkDisplayStatus-arm-linux-gnueabihf-gcc', onlyIfSuccessful: true
						cleanWs()
					}			
				}
				stage( 'Build CMake cross-compilation llvm armhf') {
					agent {
						node {
							label 'linux && development'
						}
                    }
					steps {
						sh '''
							cd CMake
							cmake -DCMAKE_TOOLCHAIN_FILE=arm-linux-llvm.cmake
							make -j4
							cp ./eLinkDisplayStatus ./eLinkDisplayStatus-arm-linux-gnueabihf-llvm
						'''
						archiveArtifacts artifacts: 'CMake/eLinkDisplayStatus-arm-linux-gnueabihf-llvm', onlyIfSuccessful: true
						cleanWs()
					}			
				}
			}
		}
	}
}
