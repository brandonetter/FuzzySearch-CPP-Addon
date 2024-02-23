declare module 'jwfsearch' {
  export default function array(
    query: string,
    documents: any[],
    fuzz: number
  ): Promise<{
    results: any[];
  }>;

  export function object(
    query: string,
    documents: any[],
    keys: string[],
    fuzz: number
  ): Promise<{
    results: any[];
  }>;
}
//yes
