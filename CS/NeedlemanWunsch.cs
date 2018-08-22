using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
 
/*
 * Eval
    mat[i][j] = max ( mat[i-1][j-1] + Eval(i,j),
 *                    mat[i-1][j] - GAP,
 *                    mat[i][j-1] - GAP )
 * )
 */
 
namespace NeedlemanWunsch
{
    public enum ChoiceOrder 
    { 
            DIAGONAL = 1,
            UP = 2,
            LEFT = 3
    }
 
    public class Aligner<T>
    {
        #region Properties
        int MISMATCH_VALUE = -1; // -2
        int MATCH_VALUE = 0; // -10
        int GAP_VALUE = -1; // -10
        int[,] matrix = null;
 
        T[] upArray = null, leftArray = null;
        public List<T> newUpList = null, newLeftList = null;
         
        int n, m, i, j;
 
        #endregion
 
        #region Methods
        public Aligner(T[] upArray, T[] leftArray)
        {
            this.upArray = upArray;
            this.leftArray = leftArray;
 
            n = leftArray.Length + 1;
            m = upArray.Length + 1;
 
            newUpList = new List<T>();
            newLeftList = new List<T>();
            FillMatrix();
        }
         
        private int Eval(int i, int j) {
 
            if (i == 0 || j == 0) {
                return matrix[0, 0];
            }
            //return leftArray[i - 1].Equals(upArray[j - 1]) ? MATCH_VALUE : MISMATCH_VALUE;
            return leftArray[i - 1].Equals(upArray[j - 1]) ? MATCH_VALUE : MISMATCH_VALUE;
        }
 
        private int Max(int a, int b, int c) {
            return (a >= b && a >= c) ? a : (b >= a && b >= c) ? b : c; 
        }
 
        private int ValueForField(int i, int j) {
            return Max(
                    matrix[i-1,j-1] + Eval(i,j),
                    matrix[i-1,j] + GAP_VALUE,
                    matrix[i,j-1] + GAP_VALUE
                );
        }
 
        private void FillMatrix() 
        { 
            matrix = new int[n,m];
             
            // Fill first row
            for(i = 1; i < n; i++){
                matrix[i, 0] = GAP_VALUE * i;
            }
 
            // Fill first column
            for (j = 1; j < m; j++) {
                matrix[0,j] = GAP_VALUE * j;
            }
 
            // Fill the rest
            for (i = 1; i < n; i++) {
                for (j = 1; j < m; j++) {
                    matrix[i, j] = ValueForField(i,j);
                }
            }
        }
 
        private bool ComesFromDiagonal(int i, int j) 
        {
            int eval = Eval(i, j);
            return (i > 0 && j > 0)? matrix[i-1,j-1] + eval == matrix[i,j] : false;
        }
 
        private bool ComesFromUp(int i, int j)
        {
            return (i > 0)? matrix[i - 1, j] + GAP_VALUE == matrix[i, j] : false;
        }
 
        private bool ComesFromLeft(int i, int j)
        {
            return (j > 0)? matrix[i, j -1] + GAP_VALUE == matrix[i, j] : false;
        }
 
        private T GetObjectForGap()
        {
            return default(T);
        }
 
        /// <summary>
        /// Returns and array of ChoiceOrder, sorted by the value
        /// </summary>
        /// <returns>An array of choice order</returns>
        public ChoiceOrder[] GetOrderChoice() {
            return (ChoiceOrder[]) Enum.GetValues(typeof(ChoiceOrder));
        }
 
        // Make a Math or a Mismatch
        private void SetsComingFromDiagonal(int row, int column)
        {
            newUpList.Insert(0, upArray[column - 1] );
            newLeftList.Insert(0,leftArray[row - 1] );
        }
 
        // Makes a gap un newUpArray
        private void SetsComingFromUp(int row, int column)
        {
            newUpList.Insert(0, GetObjectForGap());
            newLeftList.Insert(0, leftArray[row - 1]);
        }
 
        // Makes a gap un newLeftArray
        private void SetsComingFromLeft(int row, int column)
        {
            newUpList.Insert(0,upArray[column - 1]);
            newLeftList.Insert(0, GetObjectForGap());
        }
 
        public void BestAlignment() {
            i = n - 1;
            j = m - 1;
 
            bool comesFromDiagonal, comesFromUp, comesFromLeft, choiceTaken = false;
            ChoiceOrder[] choiceArray = GetOrderChoice();
            ChoiceOrder currentChoice;
            int currentChoiceIndex = 0;
                         
            while (i != 0 || j != 0)
            {
                comesFromDiagonal = ComesFromDiagonal(i, j);
                comesFromUp = ComesFromUp(i, j);
                comesFromLeft = ComesFromLeft(i, j);
                choiceTaken = false;
 
                for (currentChoiceIndex = 0; currentChoiceIndex < 3; currentChoiceIndex++)
                {
                    currentChoice = choiceArray[currentChoiceIndex];    
                    switch (currentChoice)
                    {
                        case ChoiceOrder.DIAGONAL:
                            if (comesFromDiagonal)
                            {
                                SetsComingFromDiagonal(i,j);
                                i--;j--;
                                choiceTaken = true;
                            }
                            break;
                        case ChoiceOrder.UP:
                            if (comesFromUp)
                            {
                                SetsComingFromUp(i, j);
                                i--;
                                choiceTaken = true;
                            }
                            break;
                        case ChoiceOrder.LEFT:
                            if (comesFromLeft)
                            {
                                SetsComingFromLeft(i, j);
                                j--;
                                choiceTaken = true;
                            }
                            break;
                    }
 
                    if (choiceTaken)
                    {
                        break;
                    }
                }
            }
        } 
 
        public void PrintMatrix(TextWriter writer) {
            string oneCharacter = new string(' ', 4);
            string twoCharacters = new string(' ', 3);
            string threeCharacters = new string(' ', 2);
            string fourCharacters = new string(' ', 1);
            int value, lineLength = 0;
 
            String campo = " " + oneCharacter;
            campo += campo;
            writer.Write(campo);
 
            lineLength += campo.Length;
 
            for (i = 0; i < m -1 ; i++){
                campo = upArray[i] + oneCharacter;
                writer.Write(campo);
                lineLength += campo.Length;
            }
            writer.WriteLine();
            writer.WriteLine(new String('_', lineLength));
 
            for (i = 0; i < n; i++) {
                for (j = 0; j < m; j++) {
 
                    if (j == i && i == 0) {
                        writer.Write(" " + oneCharacter);
                    }
 
                    if (j == 0 && i > 0) {
                        writer.Write(leftArray[i-1] + oneCharacter);
                    }
 
                    value = matrix[i, j];
                    int fieldSize = value.ToString().Length;
 
                    campo = value + (fieldSize == 1 ? oneCharacter : fieldSize == 2 ? twoCharacters :
                        fieldSize == 3 ? threeCharacters : fourCharacters);
 
                    writer.Write(campo);
                }
                writer.WriteLine();
                writer.WriteLine(new String('-', lineLength));
            }
            writer.WriteLine();
        }
 
        public void PrintArrays(TextWriter writer)
        {
            writer.WriteLine("\n ORIGINALS:");
            foreach (Object o in upArray)
            {
                writer.Write(o.ToString() + " ");
            }
            writer.WriteLine();
 
            foreach (Object o in leftArray)
            {
                writer.Write(o.ToString() + " ");
            }
            writer.WriteLine();
 
            writer.WriteLine("\n NEW");
            foreach (Object o in newUpList)
            {
                writer.Write(o.ToString() + " ");
            }
            writer.WriteLine();
 
            foreach (Object o in newLeftList)
            {
                writer.Write(o.ToString() + " ");
            }
            writer.WriteLine();
        }
    }
     
    public class Prueba
    {
        static void Main(string[] args)
        {
            //string target = "123467T89"; // UP
            //string source = "123456789"; // LEFT
 
            string target = "abcd"; // UP
            string source = "dcba"; // LEFT
 
            int[] arriba = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            int[] izquierda = { 1, 3, 4, 5, 7, 8, 9 };
 
            Aligner<char> aligner = new Aligner<char>(target.ToArray(), source.ToArray());
            //Aligner<int> aligner = new Aligner<int>(arriba, izquierda);
            aligner.PrintMatrix(Console.Out);
            aligner.BestAlignment();
            aligner.PrintArrays(Console.Out);
 
            Console.ReadLine();
        }
        #endregion
    }
}
