// Mesh.cs
using SharpDX;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;

namespace SoftEngine
{
    public class Mesh
    {
        public string Name { get; set; }
        public Vertex[] Vertices { get; private set; }
        public Face[] Faces { get; set; }
        public Vector3 Position { get; set; }
        public Vector3 Rotation { get; set; }

        public Texture Texture { get; set; }

        public Mesh(string name, int verticesCount, int facesCount)
        {
            Vertices = new Vertex[verticesCount];
            Faces = new Face[facesCount];
            Name = name;
        }

        public void ComputeFacesNormals()
        {
            Parallel.For(0, Faces.Length, faceIndex =>
            {
                var face = Faces[faceIndex];
                var vertexA = Vertices[face.A];
                var vertexB = Vertices[face.B];
                var vertexC = Vertices[face.C];

                Faces[faceIndex].Normal = (vertexA.Normal + vertexB.Normal + vertexC.Normal) / 3.0f;
                Faces[faceIndex].Normal.Normalize();
            });

        }

        public static Mesh LoadOBJ(string objFile)
        {
            StreamReader reader = new StreamReader(File.OpenRead(objFile));
            List<Vector3> vertices = new List<Vector3>();
            List<Vector3> normals = new List<Vector3>();
            List<Vector2> uvs = new List<Vector2>();
            List<Tuple<int, int, int>[]> faces = new List<Tuple<int, int, int>[]>();

            while (!reader.EndOfStream)
            {
                string line = reader.ReadLine();
                string[] tokens = line.Split(' ').Skip(1).SkipWhile(t => t == string.Empty).ToArray();
                if (line.StartsWith("#"))
                    continue;
                else if (line.StartsWith("vn"))
                {
                    float x = float.Parse(tokens[0]);
                    float y = float.Parse(tokens[1]);
                    float z = float.Parse(tokens[2]);
                    normals.Add(new Vector3(x, y, z));
                }
                else if (line.StartsWith("vt"))
                {
                    float x = float.Parse(tokens[0]);
                    float y = float.Parse(tokens[1]);
                    uvs.Add(new Vector2(x, y));
                }
                else if (line.StartsWith("v"))
                {
                    float x = float.Parse(tokens[0]);
                    float y = float.Parse(tokens[1]);
                    float z = float.Parse(tokens[2]);
                    vertices.Add(new Vector3(x, y, z));
                }
                else if (line.StartsWith("f"))
                {

                    Tuple<int, int, int> v1 = ParseIndicies(tokens[0]);
                    Tuple<int, int, int> v2 = ParseIndicies(tokens[1]);
                    Tuple<int, int, int> v3 = ParseIndicies(tokens[2]);
                    faces.Add(new Tuple<int, int, int>[3] { v1, v2, v3 });
                    if (tokens.Length > 3)
                    {
                        Tuple<int, int, int> v4 = ParseIndicies(tokens[3]);
                        faces.Add(new Tuple<int, int, int>[3] { v1, v3, v4 });
                    }
                }
            }

            List<Vertex> modelVerts = new List<Vertex>();
            List<Face> modelFaces = new List<Face>();
            for (int i = 0; i < faces.Count; i++)
            {
                Vertex[] faceVerts = new Vertex[3];
                for (int j = 0; j < 3; j++)
                {
                    int vertIdx = faces[i][j].Item1;
                    int uvIdx = faces[i][j].Item2;
                    int normIdx = faces[i][j].Item3;
                    faceVerts[j] = new Vertex
                    {
                        Coordinates = vertices[vertIdx],
                        Normal = (normIdx == -1) ? Vector3.Zero : normals[normIdx],
                        TextureCoordinates = (uvIdx == -1) ? Vector2.Zero : uvs[uvIdx]
                    };
                }
                modelVerts.AddRange(faceVerts);
                modelFaces.Add(new Face
                {
                    A = i * 3 + 0,
                    B = i * 3 + 1,
                    C = i * 3 + 2
                });
            }

            Mesh mesh = new Mesh("objMesh", modelVerts.Count, modelFaces.Count);

            modelVerts.CopyTo(mesh.Vertices);
            modelFaces.CopyTo(mesh.Faces);

            mesh.ComputeFacesNormals();

            return mesh;
        }

        private static Tuple<int, int, int> ParseIndicies(string s)
        {
            string[] tokens = s.Split('/');
            switch (tokens.Length)
            {
                case 1:
                    return new Tuple<int, int, int>(
                        int.Parse(tokens[0]) - 1, 
                        -1, 
                        -1);
                case 2:
                    return new Tuple<int, int, int>(
                        int.Parse(tokens[0]) - 1, 
                        int.Parse(tokens[1]) - 1, 
                        -1);
                case 3:
                    int i1 = int.Parse(tokens[0]) - 1;
                    int i2 = tokens[1] == string.Empty ? -1 : int.Parse(tokens[1]) - 1;
                    int i3 = tokens[2] == string.Empty ? -1 : int.Parse(tokens[2]) - 1;
                    return new Tuple<int, int, int>(i1, i2, i3);
                default:
                    throw new ArgumentException();
            }
        }
    }
}
