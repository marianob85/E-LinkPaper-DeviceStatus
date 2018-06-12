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
		stage('Build'){
			steps {
				sh '''
					cd Cpp
					./configure
					make
				'''

			}
		}
		stage('Package'){
			steps {
			}
		}
		
		stage('Archive'){
			steps {
				archiveArtifacts artifacts: 'Cpp/src/eLinkDisplayStatus', onlyIfSuccessful: true
			}
		}
		
		stage('CleanUp'){
			steps {
				deleteDir()
			}
		}
	}
}
