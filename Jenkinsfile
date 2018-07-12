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
						archiveArtifacts artifacts: 'autoconf/eLinkDisplayStatus', onlyIfSuccessful: true
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
							mv ./eLinkDisplayStatus ./eLinkDisplayStatus-gcc
						'''
						archiveArtifacts artifacts: 'CMake/eLinkDisplayStatus-gcc', onlyIfSuccessful: true
						cleanWs()
					}			
				}
				stage( 'Build CMake cross-compilation gcc') {
					agent {
						node {
							label 'linux && development'
						}
                    }
					steps {
						sh '''
							cd CMake
							cmake -DCMAKE_TOOLCHAIN_FILE=arm-linux-gnuaebihf.cmake .
							make -j4
							mv ./eLinkDisplayStatus ./eLinkDisplayStatus-arm-linux-gnuaebihf
						'''
						archiveArtifacts artifacts: 'CMake/eLinkDisplayStatus-arm-linux-gnuaebihf', onlyIfSuccessful: true
						cleanWs()
					}			
				}
				stage( 'Build CMake cross-compilation llvm') {
					agent {
						node {
							label 'linux && development'
						}
                    }
					steps {
						sh '''
							cd CMake
							cmake -DCMAKE_TOOLCHAIN_FILE=arm-linux-llvm.cmake .
							make -j4
							mv ./eLinkDisplayStatus ./eLinkDisplayStatus-arm-linux-llvm
						'''
						archiveArtifacts artifacts: 'CMake/eLinkDisplayStatus-arm-linux-llvm', onlyIfSuccessful: true
						cleanWs()
					}			
				}
			}
		}
	}
}
