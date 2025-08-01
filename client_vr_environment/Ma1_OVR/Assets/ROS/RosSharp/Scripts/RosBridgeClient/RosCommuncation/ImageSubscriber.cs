/*
© Siemens AG, 2017-2018
Author: Dr. Martin Bischoff (martin.bischoff@siemens.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
<http://www.apache.org/licenses/LICENSE-2.0>.
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

using UnityEngine;

namespace RosSharp.RosBridgeClient
{
    [RequireComponent(typeof(RosConnector))]
    public class ImageSubscriber : UnitySubscriber<MessageTypes.Sensor.Image>
    {
        public MeshRenderer meshRenderer;

        private Texture2D texture2D;
        private byte[] imageData;
        private uint imageWidth;
        private uint imageHeight;
        private bool isMessageReceived;

        protected override void Start()
        {
            base.Start();
            texture2D = new Texture2D(640, 480, TextureFormat.R8, false);
            meshRenderer.material = new Material(Shader.Find("Standard"));
        }
        private void Update()
        {
            if (isMessageReceived)
                ProcessMessage();
        }

        protected override void ReceiveMessage(MessageTypes.Sensor.Image compressedImage)
        {
            imageData = compressedImage.data;
            imageWidth = compressedImage.width;
            imageHeight = compressedImage.height;
            isMessageReceived = true;
        }

        private void ProcessMessage()
        {
            texture2D.LoadRawTextureData(imageData);
            //-----for compressed data
            //texture2D.LoadImage(imageData);
            texture2D.Apply();
            //-----for compressed data
            //meshRenderer.material.SetTexture("_MainTex", texture2D);
            meshRenderer.material.mainTexture = texture2D;
            isMessageReceived = false;
        }

    }
}