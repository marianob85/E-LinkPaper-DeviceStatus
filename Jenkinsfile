properties(
	[
		buildDiscarder(logRotator(artifactDaysToKeepStr: '', artifactNumToKeepStr: '', daysToKeepStr: '', numToKeepStr: '10')),
		pipelineTriggers([pollSCM('0 H(5-6) * * *')])
	]
)

pipeline
{
	agent { node { label 'linuxarmhf && development' } }
	stages
	{
		stage('Build autoconf'){
			steps {
				sh '''
					git clean -df
					cd autoconf
					autoreconf --install --force
					./configure
					make -j4
				'''
				archiveArtifacts artifacts: 'autoconf/eLinkDisplayStatus', onlyIfSuccessful: true
			}
		}
		stage('Build CMake'){
			steps {
				sh '''
					git clean -df
					cd CMake
					cmake .
					make -j4
				'''
				archiveArtifacts artifacts: 'CMake/eLinkDisplayStatus', onlyIfSuccessful: true
			}
		}
		stage('Package'){
			steps {
				sh '''
				'''
				cleanWs()
			}
		}
	}
}
