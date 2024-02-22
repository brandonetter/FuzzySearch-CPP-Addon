declare module 'jwfsearch' {
  export default function search(
    query: string,
    documents: any[],
    fuzz: number
  ): Promise<{
    results: any[];
  }>;
}
